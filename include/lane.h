#ifndef LANE_POSITION

class lane
{
    public:
    explicit lane();
    ~lane();
    unsigned int x;      //which part of picture
    unsigned int y;
    unsigned int x_line_start;  //real position 
    unsigned int x_line_end;
    unsigned int y_line_start;
    unsigned int y_line_end;   
    float density; 
    float a;
    float b;
    float new_d;
    unsigned int FA,FB,FC,FD,FE,FF;
    unsigned int  FS;
//    float TF; 

};  


#define LANE_POSITION
#endif 
