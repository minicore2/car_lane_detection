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
    float a;
    float b;

};  


#define LANE_POSITION
#endif 
