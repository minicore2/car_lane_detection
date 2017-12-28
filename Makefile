CC         = g++ -pipe

ifneq (,$(findstring run,$(firstword $(MAKECMDGOALS))))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

BINDIR     = bin

CFLAGS     = -c -g -Wall -std=c++11 -fPIC -w -O3 -msse3 -msse4.2 -funroll-loops -fexpensive-optimizations -ffast-math
LDFLAGS    =

SOURCES    = $(wildcard src/*.cpp)
OBJECTS    = $(SOURCES:.cpp=.o)

EXECUTABLE = $(BINDIR)/fc_runtime

CFLAGS     += -Iinclude

CFLAGS     += `pkg-config --cflags opencv`
LDFLAGS    += `pkg-config --libs opencv`

CFLAGS     += -I/usr/local/include/eigen
LDFLAGS    += -lboost_system -lboost_filesystem -lboost_thread -lboost_iostreams -lboost_serialization -lboost_program_options


LDFLAGS    += -L/usr/lib -L/usr/local/lib


# CFLAGS     += -shared-intel -xHost -axAVX -xSSE4.1 -unroll

LDFLAGS    += -Wl,-R\$$ORIGIN
LDFLAGS    += -Wl,-R\$$ORIGIN/..
LDFLAGS    += -Wl,-R\$$ORIGIN/../..

all: exe

exe: $(SOURCES) $(EXECUTABLE) 


$(EXECUTABLE): $(OBJECTS)
	-mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


run: all	
	LD_LIBRARY_PATH=/lib/x86_64-linux-gnu:/usr/local/lib:/usr/lib:/usr/lib/x86_64-linux-gnu:$(CURDIR)/lib:$(CURDIR)/../FCCore/bin:$(LD_LIBRARY_PATH) $(EXECUTABLE) $(RUN_ARGS)
	
clean:
	-rm -f $(OBJECTS) $(EXECUTABLE)
