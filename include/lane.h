#ifndef LANE_POSITION


enum side{left_side=1, right_side=2}; 


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
    side left_right;
//    float TF; 

};  


#define LANE_POSITION
#endif 
