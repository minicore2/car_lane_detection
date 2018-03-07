#include "cv.h"  
#include "highgui.h"  
#include "lane.h"
#include "frame.h"
#include "data_info.h"
#include <iostream>
#include <opencv2/opencv.hpp>  
using namespace cv;
using namespace std;

#define HEIGHT_NUM 10
#define WIDTH_NUM  10
#define OTSU 


#define para 0.95 
//#define test_4_debug 0 
//#define test_simple_point1 0
#define make_biger_line 
#define white_line 
#define parallel_line_while
#define whole_picture
#define debug_lane 
#define debug_video 
#define debug_detection


int middle_width=0; 

bool driving=false;
bool learning=true; 

double si2_l=0,si2_r=0; 

#ifdef debug_detection 
       std::vector<frame *>  frame_;
       std::vector<data_info *> data_info_;
#endif

extern void best_line(lane *lan) ;
extern void updateFS(lane *lan);
extern void updatepic(cv::Mat *image,lane *lan);  
extern void learning_frame(void);
extern void alert_driving(void);
extern void detection_alert(void);

int main(int argc, char *argv[])  
{  
	IplImage* img;  
        IplImage* img0;  
        IplImage* img1;  
        IplImage* img2;  
        IplImage* img3; 
        std::string pic_name="test";
        std::string pic_new="new";

        std::vector<lane *>   lane_;  
        unsigned int test_num=0;
 //       new lane;  


#ifdef debug_video 

        VideoCapture cap("video_1.mp4");// open the default camera  
        if(!cap.isOpened()) // check if we succeeded  
            return -1;
          
        namedWindow("src");
        cv::Mat temp;
        cv::Mat image; 
        cv::Mat image_origin;
//        cap >> image; // get a new frame from camera 

#else
   	cv::Mat image = cv::imread("test.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    	imshow("testSrc", image);

        cv::Mat image_origin = cv::imread("test.jpg");

#endif 

#ifdef debug_video 
//        for(int k=0;k<1 ;k++)
      for(;;)
      {
             cap >> temp;
             //cap >> image_origin;
             //if(temp.empty())
             //{           
             //    std::cout << "read image failure" << std::endl;
             //    return -1;
             //}

             cap >> image_origin;


             test_num++;
            // if(test_num>2)
            // break; 

             if(image_origin.empty())
             {
                 std::cout << "read image failure" << std::endl;
                 break; 
             }
             cv::cvtColor(image_origin,image,CV_BGR2GRAY);
             
//             imshow("src", image);
//             if(waitKey(30) >= 0)
//                break;
//        } 
//        {           
#endif 
 

#ifdef  OTSU 

        if (image.empty())
        {
                std::cout << "read image failure" << std::endl;
                return -1;
        }

        //printf("---debug--video \n");
        cv::Mat otsu;
        cv::threshold(image, otsu, 0, 255, CV_THRESH_OTSU);
        cv::imshow("otsu", otsu);


        cv::imwrite("local.jpg", otsu);
#else 
//        image_origin.at<Vec3b>(4, 4) = Vec3b(255, 255, 255);
//        dst.at<Vec3b>(50, 50) = 255, 255, 255;
//        imshow("origin", image_origin);


    	if (image.empty())
    	{
        	std::cout << "read image failure" << std::endl;
        	return -1;
    	}

    	// 局部二值化  

    	int blockSize = 25;
    	int constValue = 10;
    	cv::Mat local;
    	cv::adaptiveThreshold(image, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);

    	cv::imwrite("local.jpg", local);

    	cv::imshow("localThreshold", local);
#endif 

//        #ifdef test_4_debug 
        img = cvLoadImage("local.jpg");//默认初始图像放在工程文件下  
//        #endif 

        
	if (NULL == img)  
            return 0;  

        //img0 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存 
 
        img0 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存  
  
        cvCvtColor(img,img0,CV_BGR2GRAY);   
//图像数据复制  
//        cvCopy(img, img0, NULL);
        img1 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存  
        cvCopy(img0, img1, NULL);//数据复制，若直接赋值相当指针指向同一地址会对原本img0操作  
        img2 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存
        img3 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存
//二值化操作  
        int height = img1->height;  
        int width = img1->width;  
        middle_width= img1->width/2; 
//        int step  = img1->width;
        int step = img1->widthStep;  
        int channels = img1->nChannels; 
        bool on=true;
  
//        cout<<"size : height="<<img1->height<<"  width="<<img1->width<<" widthStep="<<img1->widthStep<<" nchannels="<<img1->nChannels<<std::endl; 

        uchar *data = (uchar*)img1->imageData;  
        uchar *data2 = (uchar*)img2->imageData; 

        for(int i=0;i != height; ++ i)  
        {  
         for(int j=0;j != width; ++ j)  
         {  
             for(int k=0;k != channels; ++ k)  
             {  
                 if(data[i*step+j*channels+k]<128)  
                 {
                    data[i*step+j*channels+k]=0;//255-data[i*step+j*channels+k];  
//                   if(on==true)
//                   {
//                     printf("---value= %d  num=%d i=%d,j=%d\n",data[i*step+j*channels+k],i*step+j*channels+k,i,j);
//                      on=false;
//                   }
                 }
                 else  
                 {
                  #ifdef test_simple_point1
                  data[i*step+j*channels+k]=0;
                  #else
                  data[i*step+j*channels+k]=255;//255-data[i*step+j*channels+k];  
                  #endif
                 }
             }  
         }  
        } 

// removing noise

//data[4*step+118]=255;

#ifdef test_simple_point1
data[4*step+5]=255;
data[8*step+19]=255;
data[9*step+13]=255;
data[9*step+25]=255;
data[10*step+6]=255;
data[14*step+20]=255;
data[16*step+27]=255;
data[19*step+22]=255;

data[19*step+80]=255;
//data[15*step+111]=255;
#endif


/*
data[21*step+9]=255;
data[26*step+7]=255;
data[6*step+37]=255;
data[5*step+48]=255;
data[14*step+20]=255;
data[19*step+22]=255;
data[9*step+25]=255;
data[16*step+27]=255;

data[3*step+80]=255;
*/


//data[7*step+122]=255;
//data2[7*step+122]=255;


//        data2[1*step+0]=255;       
//        data2[1*step+1]=255;
//        data2[1*step+2]=255;

#ifdef test_4_debug

        data2[23*step+84]=0;
        data2[23*step+85]=0;
        data2[4*step+106]=0;
        data2[3*step+106]=0;
        data2[3*step+105]=0;
 //       data2[3*step+104]=0;
        data2[2*step+104]=0;

        data[23*step+84]=0;
        data[23*step+85]=0;
        data[4*step+106]=0;
        data[3*step+106]=0; 
        data[3*step+105]=0;
 //       data[3*step+104]=0;
        data[2*step+104]=0;

//        data[4*step+132]=255;
//        data2[4*step+132]=255;
//        data[5*step+132]=255;
//        data2[5*step+132]=255;

#endif 

/*
       data[4*step+87]=255;
       data[5*step+87]=255;
       data[7*step+87]=255;
       data[6*step+87]=255;

       data2[4*step+87]=255;
       data2[5*step+87]=255;
       data2[6*step+87]=255;
       data2[7*step+87]=255;

*/



        cvCopy(img1, img2, NULL);
        cvCopy(img1, img3, NULL);

        
        for(int i=0;i<HEIGHT_NUM;i++)
        {
        
        for(int j=0;j<WIDTH_NUM;j++)        
        {

//        int i=4,j=0;  
//        printf("---- part x=%d,y=%d \n",i,j);
       
        lane *lan=new lane(); 


        int height_start=  (img1->height/HEIGHT_NUM)*i, height_end=  (img1->height/HEIGHT_NUM)*(i+1); 
        int width_start=   (img1->width/WIDTH_NUM)*j, width_end=     (img1->width/WIDTH_NUM)*(j+1);


//        int height_start=  0, height_end=  img1->height ; 
//        int width_start=   0, width_end=   img1->width  ; 

//        printf("--test height_start=%d,height_end=%d \n", height_start,height_end);
//        printf("--test width_start=%d, width_end=%d \n",  width_start,width_end);


        lan->x=i;
        lan->y=j;
        lan->x_line_start=height_start;
        lan->x_line_end=height_end;
        lan->y_line_start=width_start;
        lan->y_line_end=width_end;
     

        float total_num=0,data_num=0;

        double xl=0,yl=0 ;

        int num=0; 

        float a=0,b=0;          

	float  dl=0,d=0;

        float new_d=0,last_d;

        float si=0,si2=0,last_si2=0;;

        float bench =0;

     //   printf("--test here \n");
        for(int itera=0;itera<8;itera++)        
        {

        num=0;
        xl=0;
        yl=0;                 

       for(int x=height_start;x !=height_end; x++)
       {
            for(int y=width_start;y !=width_end; y++)
            {
                   if(255==data[x*step+y])
                   {
                      data_num++;
                   }
                   total_num++;

            }
       }

       lan->density=data_num/total_num; 


        for(int x=height_start;x !=height_end; ++x)
        {
      
           for(int y=width_start;y !=width_end; ++y)  
           {
               
//               printf("--- test x=%d,y=%d,data=%d \n",x,y,data[x*step+y]);
               if(255==data[x*step+y])    
               {

                       num++;
                       xl+=x;
                       yl+=y;
//                       printf(" --- line-point x=%d y=%d xl=%g yl=%g num=%d \n",x,y,xl,yl,num);  
               }
           }
        }

        if(0==num)
        {
            lane_.push_back(lan);
            break; 
        }
 
        double xi=0 , yi=0 ;  

        xi=xl/num ;
        yi=yl/num ; 

 //       printf("--- c-point xi=%g,yi=%g num=%d \n",xi,yi,num); 


        double ax=0,bx=0;
       
        for(int x=height_start;x !=height_end; ++x)
        {
           for(int y=width_start;y !=width_end; ++y)
           {
               if(255==data[x*step+y])
               {
                        ax+=(x-xi)*x ;
                        bx+=(y-yi)*x ;
//                        printf(" --- line-b x=%d y=%d aa=%f bb=%f  \n",x,y,(x-xi)*x,(y-yi)*x);
//                        printf(" --- line-b x=%d y=%d ax=%f bx=%f  \n",x,y,ax,bx);
               }
           }
        }


       b=bx/ax;                 
        
       a=yi-b*xi;

  //     printf(" ---- linx y= %f+%fx \n", a,b); 

       dl=0; 

       unsigned int dl_num=0;
       float dx=0,dy=0,di;

       for(int x=height_start;x !=height_end; ++x)
        {
           for(int y=width_start;y !=width_end; ++y)
           {
               if(255==data[x*step+y])
               {
                       dl_num++;
                       dx=abs(b*x-y+a);
                       dy=sqrt(b*b+1);
                       dl+=dx/dy ;
//                       printf("--- x=%d,y=%d  distance=%g \n",x,y,dx/dy);
               }
            }

        }
         
       di=dl/dl_num;

 //      printf("--- di=%g dl=%g dl_num=%d \n",di,dl,dl_num);


       unsigned int test_x=20,test_y=23; 
       float test_distance=0;

/*
 //      dx=abs(b*test_x-test_y+a);
       dx=b*test_x-test_y+a; 
       dy=sqrt(b*b+1);
       test_distance=dx/dy ;
      // data[15*step+78]=255;

       printf("---- test distance\n");
       printf("---- line y= %f+%fx \n", a,b);
       printf("---- point x=%d,y=%d test_d=%g \n",test_x,test_y,test_distance);
 */    

      unsigned s_num=0;

      for(int x=height_start;x !=height_end; ++x)
      {
           for(int y=width_start;y !=width_end; ++y)
           {
                   if(255==data[x*step+y])
                   {
                       dx=abs(b*x-y+a);
                       dy=sqrt(b*b+1);
                       d=dx/dy ;
//                       printf("--test-- d=%g \n",d);
                       si+=(d-di)*(d-di);
//                       si+=abs(d-di);
                   }
           }

       }

       if(0!=itera)
       {
           last_si2=si2; 
       } 

       si2=si/(num-1);

       if((last_si2/si2)>=2)
       break; 
   

//       printf("--- si=%g  si2=%g  \n",si,si2);

       last_d=new_d ;

       new_d=sqrt(si2)/0.5;      
  
//       new_d=si2; 

//       float total_num=0,data_num=0;      

       float space_ori=0;
 
       float space_lat=0;

       bool  change_map=true;

       while((true==change_map)&&(0!=itera))
       {

//       printf("--test--- new_d %g \n",new_d);

       data_num=0;

       total_num=0; 

       for(int x=height_start;x !=height_end; x++)
       {
            for(int y=width_start;y !=width_end; y++)
            {
                dx=abs(b*x-y+a);
                dy=sqrt(b*b+1);
                d=dx/dy ;
                if(d<=last_d)
                {

//                   printf("--test-- great---\n");
                   if(255==data[x*step+y])
                   {
                      data_num++; 
                   }
                   total_num++;
                   
                }           
            }
       }

      space_ori=data_num/total_num; 

  //    printf("--test-- space_ori=%g data_num=%d total_num=%d \n",space_ori,data_num,total_num);

      data_num=0;
      total_num=0; 

      for(int x=height_start;x !=height_end; x++)
       {
            for(int y=width_start;y !=width_end; y++)
            {
                dx=abs(b*x-y+a);
                dy=sqrt(b*b+1);
                d=dx/dy ;
                if(d<=new_d)
                {  
                   if(255==data[x*step+y])
                   {
                      data_num++; 
                   }
                   total_num++;
                   
                }
            }
       }

       space_lat=data_num/total_num;

    //   printf("--test-- space_ori=%g space_lat=%g ",space_ori,space_lat);

       if(space_lat<=space_ori)
       {
           change_map=false; 
           new_d=new_d*1.1;
//           new_d=new_d*(para+0.10);
       }
       else
       {
           change_map=false; 
       }

       }

     
      

       lan->a=a;
       lan->b=b;
       lan->new_d=new_d; 
//       lane_.push_back(lan);
    

   //    printf("--- new_d %g \n",new_d); 


       cvCopy(img3, img2, NULL);       
     

       //data2[15*step+78]=255; 
       for(int x=height_start;x !=height_end; x++)
        {
           for(int y=width_start;y !=width_end; y++)
           {
               if(int(a+b*x)==y)
               {
                  #ifdef make_biger_line
                  if(y>(height_start+3)&&y<height_end)
                   {
                       data2[y*step+x+2]=255;
                       data2[y*step+x+1]=255;
                       data2[y*step+x-1]=255;
                       data2[y*step+x-2]=255;
                   }
                   #endif

                   #ifdef  test_simple_point1
                   data2[x*step+y]=255;
                   #else 
                
                   #ifdef  white_line
                   data2[x*step+y]=255;
                   #else
                   data2[x*step+y]=0;
                   #endif                    
             
                   #endif                   
               }

               dx=abs(b*x-y+a);
               dy=sqrt(b*b+1);
               d=dx/dy ;
               if((int)d==(int)new_d)
               {
                   if(0==data2[x*step+y])
                   {
                       data2[x*step+y]=255;
                   }
                   else
                   {  
                       data2[x*step+y]=0;
                   }
               }

            }
        }

        cvCopy(img3, img1, NULL);

#ifndef whole_picture 
       for(int x=0;x !=height; x++)
        {
            for(int y=0;y !=width; y++)
            {
                if(!((x>height_start)&&(x<height_end)))
                {
                    data[x*step+y]=0;
                }

                if(!((y>width_start)&&(y<width_end)))
                {
                    data[x*step+y]=0;
                }
            }
        }
#endif 

        if(7==itera)
        {
             lane_.push_back(lan);
//             delete lan;  
        } 



       if(7==itera)
       {

       for(int x=height_start;x <=height_end; x++)
        {
           for(int y=width_start;y <=width_end; y++)
           {
                #ifdef debug_lane
               if((width_end==y)||(height_end==x))
               {
                   image_origin.at<Vec3b>(x, y) = Vec3b(0, 0, 255);
               }
               #endif 
      

/* 
               if(int(a+b*x)==y)
               {
       //lan->a=a;
       //lan->b=b;
       //lane_.push_bacK(lan);                              
                 
                   if((space_ori>0.70)||(space_ori<0.1))
                       break;  
                   if(y<(img1->width/2))
                   {
                       if(b>0)
                       break;
                   }
                   else
                   {
                       if(b<0)
                       break ; 

                   }    

                  #ifdef make_biger_line
                //  if(y>(height_start+3)&&y<height_end)
                   {
                       image_origin.at<Vec3b>(x, y-2) = Vec3b(0, 0, 255);
                       image_origin.at<Vec3b>(x, y-1) = Vec3b(0, 0, 255);
                       image_origin.at<Vec3b>(x, y) = Vec3b(0, 0, 255);
                       image_origin.at<Vec3b>(x, y+1) = Vec3b(0, 0, 255);
                       image_origin.at<Vec3b>(x, y+2) = Vec3b(0, 0, 255);
                    
                   }
                   #else
                   image_origin.at<Vec3b>(x, y) = Vec3b(0, 0, 255);
                   #endif 
               }

  */          }
        }
       
        }

     //   double dx=0,dy=0;
        for(int x=height_start;x !=height_end; x++)
        {
            for(int y=width_start;y !=width_end; y++)
            {     
                dx=abs(b*x-y+a);
                dy=sqrt(b*b+1);
                d=dx/dy ;      
                if(d>new_d)
                data[x*step+y]=0;
            }
        }

#ifndef whole_picture
        pic_name+="1" ;
        pic_new+="1";
        cvNamedWindow(pic_name.c_str(), 0 );
        cvShowImage(pic_name.c_str(), img2);
        cvNamedWindow(pic_new.c_str(), 0 );
        cvShowImage(pic_new.c_str(), img1);

#endif
 
        }
     
       }

       }       

   //   y=a+bx



       std::vector<lane *>::iterator lis; 
       std::vector<lane *>::iterator lit;


       for(lis=lane_.begin();lis!=lane_.end();++lis)
       {
  
           best_line(*lis);

       }
       
       for(lis=lane_.begin();lis!=lane_.end();++lis)
       {
           for(lit=lane_.begin();lit!=lane_.end();++lit)
           {
//              printf("----shinq-- x=%d,y=%d FS=%d  \n",(*lis)->x,(*lis)->y,(*lis)->FS);

               if((*lis)->FS>=4)
               {

//                  printf("----FS>2 \n");
 
                   int lane_middle_width1=((*lis)->y_line_end+(*lis)->y_line_start)/2;
                   int lane_middle_width2=((*lis)->y_line_end+(*lis)->y_line_start)/2; 
 
                   if(((lane_middle_width1<middle_width)&&(lane_middle_width2<middle_width))||\
                   ((lane_middle_width1>middle_width)&&(lane_middle_width2>middle_width)))
                   {                        


                   if(((*lis)->x!=(*lit)->x)&&((*lis)->y!=(*lit)->y))
                   {
                       
//                       printf("----abs((*lis)->b-(*lit)->b)=%g  \n",abs((*lis)->b-(*lit)->b));
                       if((abs((*lis)->b-(*lit)->b)<=0.1)&&abs((*lis)->a-(*lit)->a)<=50) 
                       {
                          (*lis)->FE++;                        
            
                       }          
                  //     if(abs((*lis)->a-(*lit)->a)<=30)
                  //     {
                  //        (*lis)->FF++;
                  //     }
                   } 


                   }
               }
           }
       }

       
      bool set_left=false,set_right=false,set_frame=false;

      for(lis=lane_.begin();lis!=lane_.end();++lis)
      {
           updateFS(*lis);
  //         printf("----shinq-- x=%d,y=%d FS=%d  \n",(*lis)->x,(*lis)->y,(*lis)->FS);

           if((*lis)->FS>=5)
           {

               #ifdef debug_detection

               frame *fram=new frame(); 

               switch((*lis)->left_right)
               {
                   case right_side :
                        if(false==set_right)
                        {
                            fram->best_b_r=(*lis)->b;     
                            set_right=true;
                            printf(" set right best_b=%g \n" ,fram->best_b_r);
                        }
                        break ;                
                   case left_side :
                        if(false==set_left)
                        {
                            fram->best_b_l=(*lis)->b; 
                            set_left=true; 
                            printf(" set left best_b=%g \n",fram->best_b_l);
                        }
                        break; 
                   default :
                        break; 
               }

               if((true==set_right)&&(true==set_left)&&(false==set_frame))
               {
                   frame_.push_back(fram);
                   set_frame=true; 
               }
               #endif 
               updatepic(&image_origin,*lis);            

           }
       }

      imshow("origin", image_origin);

      lane_.clear();

      cvWaitKey(0);

      #ifdef debug_video
      if(waitKey(60) >= 0)
          break;
      }
      #endif

      #ifdef debug_detection
      printf("  frame number=%d \n",frame_.size());
      detection_alert();
      #endif
      printf("   si2_l=%g,si2_r=%g \n",si2_l,si2_r);
 
//创建窗口、显示图像、销毁图像、释放图像  
//        cvNamedWindow( "test1", 0 );  
//        cvShowImage("test1", img0);  

        cap.release();
 
        cvWaitKey(0);  
  
//        cvDestroyWindow( "test1" );  
        cvDestroyWindow( "test1" );  
  
        cvReleaseImage( &img0 );  
        cvReleaseImage( &img1 );  
  
        return 0;  

}

void detection_alert(void)
{

    //#ifdef debug_detection 

       if(true==driving)
       {
            
               alert_driving();    

       }
       else
       {
               if(true==learning)
               {
                       printf("---shinq-- learning = true \n");
                       learning_frame(); 
               }
       }

    //#endif

}


void learning_frame(void)
{

       std::vector<frame *>::iterator lis;
       std::vector<data_info *>::iterator lit;
       double  data_l=0,data_r=0,data_m_l=0,data_m_r=0,s_m_l=0,s_m_r=0,si2_m_l=0,si2_m_r=0; 
       int num=0;

       for(lis=frame_.begin();lis!=(frame_.end()-2);++lis)
       {        
            printf("--shinq best_b_l+2 =%g,best_b_l=%g \n",(*(lis+2))->best_b_l,(*lis)->best_b_l);          
            data_info *data=new data_info();            
            data->speed_l= (*(lis+2))->best_b_l- (*lis)->best_b_l;           
            data->speed_r= (*(lis+2))->best_b_r- (*lis)->best_b_r;
            data_info_.push_back(data); 
       }        

       for(lit=data_info_.begin();lit!=data_info_.end();++lit)
       {
            data_l+=(*lit)->speed_l ;      
            data_r+=(*lit)->speed_r ; 
            num++;
            printf(" data_l=%g,data_r=%g num=%d \n",data_l,data_r,num); 
       }

       data_m_l=data_l/num;
       data_m_r=data_r/num; 

       printf(" data_m_l=%g,data_m_r=%g \n",data_m_l,data_m_r);

       for(lit=data_info_.begin();lit!=data_info_.end();++lit)
       {
            s_m_l+=((*lit)->speed_l-data_m_l)*((*lit)->speed_l-data_m_l);
            s_m_r+=((*lit)->speed_r-data_m_r)*((*lit)->speed_r-data_m_r);
             printf(" speed_l=%g,speed_r=%g \n",(*lit)->speed_l,(*lit)->speed_r);
             printf(" diff_l=%g,diff_r=%g \n",((*lit)->speed_l-data_m_l)*((*lit)->speed_l-data_m_l),((*lit)->speed_r-data_m_r)*((*lit)->speed_r-data_m_r));
             printf("  s_m_l=%g, s_m_r=%g  \n",s_m_l,s_m_r); 
       } 

       printf("  s_m_l=%g,sm_r=%g \n",s_m_l,s_m_r);


       si2_m_l=s_m_l/(num-1); 
       si2_m_r=s_m_r/(num-1);


       printf("  si2_m_l=%g,si2_m_r=%g \n",si2_m_l,si2_m_r); 

       si2_l=sqrt(si2_m_l);
       si2_r=sqrt(si2_m_r); 

       printf("  si2_l=%g,si2_r=%g \n",si2_l,si2_r);
}


void alert_driving(void)
{



}


void best_line(lane *lan)
{ 

    int lane_middle_width=0; 

    lane_middle_width=(lan->y_line_end+lan->y_line_start)/2; 


//    printf(" lan->y_line_end=%d lan->y_line_start=%d \n",lan->y_line_end,lan->y_line_start);
//    printf(" lane_middle_width=%d middle_width=%d \n",lane_middle_width,middle_width);



    if(lane_middle_width<middle_width)
    {
        lan->left_right=left_side;

        if(lan->b<0)
            lan->FA=1;

        if(lan->b<-0.176)
        { 
            lan->FB=1; 
        }
        else
        {
            lan->FS=0;
            return;
        }
    }
    else
    {
        lan->left_right=right_side; 

 //       printf(" x=%d y=%d right \n",lan->x,lan->y);

        if(lan->b>0)
            lan->FA=1;
 
        if(lan->b>0.176)
        {   
            lan->FB=1; 
        }
        else
        {
            lan->FS=0;
            return;
        }
    }

    if((lan->density<0.70)||(lan->density>0.1))
        lan->FD=1;

    updateFS(lan);


    if(lan->FS>=3)
    {
        if(lan->new_d<(lan->x_line_end-lan->x_line_start)/4)
        {
            lan->FF=1;      
        }
    }

    updateFS(lan);

    return ; 
}


void updateFS(lane *lan)
{
    lan->FS=(lan->FA+lan->FB+lan->FC+lan->FD+lan->FE+lan->FF);

}
 

void updatepic(cv::Mat *image,lane *lan)
{
        float dx=0,dy=0,d=0; 
         
        for(int x=lan->x_line_start;x <=lan->x_line_end; x++)
        {
           for(int y=lan->y_line_start;y <=lan->y_line_end; y++)
           {
       //        if((width_end==y)||(height_end==x))
       //        {
       //            image_origin.at<Vec3b>(x, y) = Vec3b(0, 0, 255);
       //        }
            
               if(int(lan->a+lan->b*x)==y)
               {
       //lan->a=a;
       //lan->b=b;
       //lane_.push_bacK(lan);                              
                 
       //            if((space_ori>0.70)||(space_ori<0.1))
      //                 break;  
      //             if(y<(img1->width/2))
      //             {
      //                 if(b>0)
     //                  break;
     //              }
     //              else
     //              {
     //                  if(b<0)
     //                  break ; 

      //             }    

                  #ifdef make_biger_line
                //  if(y>(height_start+3)&&y<height_end)
                   {
                       image->at<Vec3b>(x, y-2) = Vec3b(0, 0, 255);
                       image->at<Vec3b>(x, y-1) = Vec3b(0, 0, 255);
                       image->at<Vec3b>(x, y) = Vec3b(0, 0, 255);
                       image->at<Vec3b>(x, y+1) = Vec3b(0, 0, 255);
                       image->at<Vec3b>(x, y+2) = Vec3b(0, 0, 255);
                    
                   }
                   #else
                   image->at<Vec3b>(x, y) = Vec3b(0, 0, 255);
                   #endif 
               }

                #ifdef debug_lane
                dx=abs(lan->b*x-y+lan->a);
                dy=sqrt(lan->b*lan->b+1);
                d=dx/dy ;
                if(int(d)==int(lan->new_d))
                {
                    image->at<Vec3b>(x, y) = Vec3b(0, 0, 255);
                }
                #endif
            }
        }



}
