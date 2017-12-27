#include "cv.h"  
#include "highgui.h"  
//#include <cvaux.h>  
#include <iostream>

using namespace std;

  
int main(int argc, char *argv[])  
{  
        IplImage* img;  
        IplImage* img0;  
        IplImage* img1;  
  
        img = cvLoadImage("test-2.jpg");//默认初始图像放在工程文件下  
        //IplImage* img1 = img;  
  
        if (NULL == img)  
            return 0;  
  
//灰度化操作  
        img0 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存  
  
//        cout<<"get img size="<<cvGetSize(img)<<endl ;

        cvCvtColor(img,img0,CV_BGR2GRAY);  
//图像数据复制  
        img1 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);//申请一段内存  
        cvCopy(img0, img1, NULL);//数据复制，若直接赋值相当指针指向同一地址会对原本img0操作  
  
  
//二值化操作  
        int height = img1->height;  
        int width = img1->width;  
        int step = img1->widthStep;  
        int channels = img1->nChannels; 
        bool on=true;
  
        cout<<"size : height="<<img1->height<<"  width="<<img1->width<<" widthStep="<<img1->widthStep<<" nchannels="<<img1->nChannels<<std::endl; 

        uchar *data = (uchar*)img1->imageData;  
  
        for(int i=0;i != height; ++ i)  
        {  
         for(int j=0;j != width; ++ j)  
         {  
             for(int k=0;k != channels; ++ k)  
             {  
                 if(data[i*step+j*channels+k]<128)  
                 {
                  data[i*step+j*channels+k]=0;//255-data[i*step+j*channels+k];  
//                  if(on==true)
//                  {
//                      printf("---value= %d  num=%d i=%d,j=%d\n",data[i*step+j*channels+k],i*step+j*channels+k,i,j);
//                      on=false;
 //                 }
                 }
                 else  
                  data[i*step+j*channels+k]=255;//255-data[i*step+j*channels+k];  
             }  
         }  
        } 

//        printf("value= %d\n",data[523*step+175]);

        unsigned long int xl=0,yl=0,num=0; 
        
             
        for(int i=0;i !=height; ++i)
        {
           for(int j=0;j !=width; ++j)  
           {
               if(255==data[i*step+j])    
               {
                   num++;
                   xl+=i;
                   yl+=j;
                   printf(" --- line-point x=%d y=%d xl=%d yl=%d num=%d \n",i,j,xl,yl,num);  
                   

               }
           }
        }


        float xi=0 , yi=0 ;  

        xi=xl/num ;
        yi=yl/num ; 

//        printf("--- c-point xi=%f,yi=%f \n",xi,yi); 


        unsigned long int ax=0,bx=0;
         
        for(int i=0;i !=height; ++i)
        {
           for(int j=0;j !=width; ++j)
           {
               if(255==data[i*step+j])
               {
                    ax+=(i-xi)*i ;
                    bx+=(j-yi)*i ;
                    printf(" --- line-b x=%d y=%d ax=%d bx=%d  \n",i,j,ax,bx);
           
               }
           }
        }

        float b=0 ,a=0;

        b=bx/ax;                 
        
        a=yi-b*xi;

        printf("--- c-point xi=%f,yi=%f \n",xi,yi);
        printf(" ---- linx y= %f+%fx \n", a,b);


//创建窗口、显示图像、销毁图像、释放图像  
        cvNamedWindow( "test1", 0 );  
        cvShowImage("test1", img0);  
  
        cvNamedWindow( "test", 0 );  
        cvShowImage("test", img1);  
  
        cvWaitKey(0);  
  
        cvDestroyWindow( "test1" );  
        cvDestroyWindow( "test" );  
  
        cvReleaseImage( &img0 );  
        cvReleaseImage( &img1 );  
  
        return 0;  

}  
