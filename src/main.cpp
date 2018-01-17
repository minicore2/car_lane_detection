#include "cv.h"  
#include "highgui.h"  
#include "lane.h"
#include <iostream>
#include <opencv2/opencv.hpp>  
using namespace cv;
using namespace std;


//#define test_4_debug 0 
//#define test_simple_point1 0
#define make_biger_line 0

  
int main(int argc, char *argv[])  
{  
	IplImage* img;  
        IplImage* img0;  
        IplImage* img1;  
        IplImage* img2;  
        IplImage* img3; 
        std::string pic_name="test";
        std::string pic_new="new";

        std::vector<lane *> lane_;  
 //       new lane;  


   	cv::Mat image = cv::imread("test-14.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    	imshow("testSrc", image);

        cv::Mat image_origin = cv::imread("test-14.jpg");
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
//        int step  = img1->width;
        int step = img1->widthStep;  
        int channels = img1->nChannels; 
        bool on=true;
  
        cout<<"size : height="<<img1->height<<"  width="<<img1->width<<" widthStep="<<img1->widthStep<<" nchannels="<<img1->nChannels<<std::endl; 

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

        
    //    for(int i=0;i<5;i++)
//        {
        
    //    for(int j=0;j<5;j++)        
//        {
      
        int i=3,j=1;
  
        printf("---- part x=%d,y=%d \n",i,j);
       
        lane *lan=new lane(); 


        int height_start=  (img1->height/5)*i, height_end=  (img1->height/5)*(i+1); 
        int width_start=   (img1->width/5)*j, width_end=     (img1->width/5)*(j+1);


        printf("--test height_start=%d,height_end=%d \n", height_start,height_end);
        printf("--test width_start=%d, width_end=%d \n",  width_start,width_end);


        lan->x=i;
        lan->y=j;
        lan->x_line_start=height_start;
        lan->x_line_end=height_end;
        lan->y_line_start=width_start;
        lan->y_line_end=width_end;
     

        double xl=0,yl=0 ;

        int num=0; 

        float a=0,b=0;          

	float  dl=0,d=0;

        float new_d=0;

        float bench =0;

        printf("--test here \n");
        for(int itera=0;itera<8;itera++)        
        {

        num=0;
        xl=0;
        yl=0;                 

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
        break; 

        double xi=0 , yi=0 ;  

        xi=xl/num ;
        yi=yl/num ; 

        printf("--- c-point xi=%g,yi=%g num=%d \n",xi,yi,num); 


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

       printf(" ---- linx y= %f+%fx \n", a,b); 

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

       printf("--- di=%g dl=%g dl_num=%d \n",di,dl,dl_num);


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

      float si=0,si2=0;
      unsigned s_num=0;

      for(int x=height_start;x !=height_end; ++x)
      {
           for(int y=width_start;y !=width_end; ++y)
           {
                   if(255==data[x*step+y])
                   {
                       dx=b*x-y+a;
                       dy=sqrt(b*b+1);
                       d=dx/dy ;
//                       printf("--test-- d=%g \n",d);
                       si+=(d-di)*(d-di);
//                       si+=abs(d-di);
                   }
           }

       }

       si2=si/(num-1);

       printf("--- si=%g  si2=%g  \n",si,si2);

       if(0==itera)
       {
           bench=sqrt(si2);
           new_d=sqrt(si2);
       }
       else
       {
           new_d=si2; 
       }

       
       lan->a=a;
       lan->b=b; 
       lane_.push_back(lan);
    

       printf("--- new_d %g \n",new_d); 


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
                       data[y*step+x+2]=255;
                       data[y*step+x+1]=255;
                       data[y*step+x-1]=255;
                       data[y*step+x-2]=255;
                   }
                   #endif

                   #ifdef  test_simple_point1
                   data2[x*step+y]=255;
                   #else 
                   data2[x*step+y]=255;
                   #endif 
                   
               }
            }
        }

        cvCopy(img3, img1, NULL);


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

        pic_name+="1" ;
        pic_new+="1";
        cvNamedWindow(pic_name.c_str(), 0 );
        cvShowImage(pic_name.c_str(), img2);
        cvNamedWindow(pic_new.c_str(), 0 );
        cvShowImage(pic_new.c_str(), img1);


        if(new_d<bench)
        break; 
//        cvCopy(img1, img2, NULL); 
        }
     
  //      }

 //       }       

   //   y=a+bx
 
//        line(image_origin,Point(width_start,(width_start-a)/b),Point(width_end,(width_end-a)/b),Scalar(0,0,255),5,CV_AA);

 //       printf(" test line start (%d,%d) end (%d,%d) \n",width_start,(width_start-a)/b,width_end,(width_end-a)/b);
        imshow("origin", image_origin);


//创建窗口、显示图像、销毁图像、释放图像  
//        cvNamedWindow( "test1", 0 );  
//        cvShowImage("test1", img0);  
 
        cvWaitKey(0);  
  
//        cvDestroyWindow( "test1" );  
        cvDestroyWindow( "test1" );  
  
        cvReleaseImage( &img0 );  
        cvReleaseImage( &img1 );  
  
        return 0;  

}  
