#include "utils.h"
#include "node.h"
#include "source.h"
#include "sink.h"
#include "sourcesink.h"
#include "frame.h"

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string.h>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include <chrono>
#include <thread>

#include <sys/statvfs.h>

#include "opencv2/opencv.hpp"

#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_io.hpp"

volatile sig_atomic_t done = 0;

inline static void restart(char** args) {
	int pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Restart failed!\n");
	} else if (pid == 0) {
		fprintf(stdout, "Trying to restart at %d\n", getpid());
		execve(args[0], args, NULL);
		fprintf(stderr, "Failed to restart!\n");
		_exit(-1);
	} else {
        fprintf(stdout, "Killing old process at %d\n", getpid());
		done = 1;
	}
}

inline static time_t get_mtime(const char* path) {
    struct stat attrib;
    stat(path, &attrib);
    return attrib.st_ctime;
}

int main(int argc, char** argv) {
    std::string type;
    std::string host;
    std::string next;

    try {
        boost::program_options::options_description desc("formosa");
        desc.add_options()
            ("type,t", boost::program_options::value<std::string>(&type)->default_value("node"), "Type")
            ("host,h", boost::program_options::value<std::string>(&host)->default_value("localhost"), "Current Host Name")
            ("next,n", boost::program_options::value<std::string>(&next)->default_value("localhost"), "Next Host Name")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
        boost::program_options::notify(vm);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        throw std::runtime_error("Command line cannot be parsed!");
    }

    formosa::Node* node = NULL;
    if (boost::iequals(type, "source")) {
        node = new formosa::Source(next.c_str(), host.c_str());
    } else if (boost::iequals(type, "sink")) {
        node = new formosa::Sink(next.c_str(), host.c_str());
    } else if (boost::iequals(type, "sourcesink")){
        node = new formosa::SourceSink(next.c_str(), host.c_str());
    } else {
        node = new formosa::Node(next.c_str(), host.c_str());
    }

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = [] (int s) { done = 1; };
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    time_t mtime = get_mtime("./libfc_node.so");
    double dmtime = 0.0;

    std::cout << "Node " << node->id() << ":" << node->uid() << " started." << std::endl;

    boost::posix_time::time_facet *facet;
    facet = new boost::posix_time::time_facet("%H:%M:%S");
    std::cout.imbue(std::locale(std::cout.getloc(), facet));

    while (!done) {
        std::cout << "\033[0;36m" << "[" << boost::posix_time::second_clock::local_time() << "] " << "\e[0m";

        int ret = (*node)();

        if (ret > 0) {
            std::cout << "\033[0;31m" << ret << " subject(s) detected." << "\e[0m";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else if (ret == 0) {
            std::cout << "\033[0;32m" << "No subject detected." << "\e[0m";
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } else {
            std::cout << "\033[0;33m" << "No frame retrieved." << "\e[0m";
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        std::cout << std::endl;

        time_t tmp = get_mtime("./libfc_node.so");
        dmtime = difftime(tmp, mtime);
        mtime = tmp;
        // if (dmtime > 0.0) restart(argv);
        if (dmtime > 0.0) {
            std::cout << "Node " << node->id() << ":" << node->uid() << " stopped." << std::endl;
            delete node;
            execve(argv[0], argv, NULL);
        }
    }

    if (node != NULL) {
        std::cout << "Node " << node->id() << ":" << node->uid() << " stopped." << std::endl;
        delete node;
    }

    return 0;
}
