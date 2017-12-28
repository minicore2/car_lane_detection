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

        bool init=true;
        unsigned long int xl=0,yl=0,num=0; 
        float b=0 ,a=0;    
        double d=0, di=0,dx=0,dy=0;
        double new_d=0 ;
        
        for(int itera=0;itera<2;itera++)        
        {


        for(int y=0;y !=height; ++y)
        {
           for(int x=0;x !=width; ++x)  
           {
               if(255==data[y*step+x])    
               {

                   dx=abs(b*y-x+a);
                   dy=sqrt(b*b+1);
                   d=dx/dy ;
                   if((d<=new_d)&&(init==true))
                   {

                       num++;
                       xl+=x;
                       yl+=y;
//                       printf(" --- line-point x=%d y=%d xl=%d yl=%d num=%d \n",x,y,xl,yl,num);  
                   
                   }

               }
           }
        }


        float xi=0 , yi=0 ;  

        xi=xl/num ;
        yi=yl/num ; 

        printf("--- c-point xi=%f,yi=%f \n",xi,yi); 


        float ax=0,bx=0;
         
        for(int y=0;y !=height; ++y)
        {
           for(int x=0;x !=width; ++x)
           {
               if(255==data[y*step+x])
               {
                   dx=abs(b*y-x+a);
                   dy=sqrt(b*b+1);
                   d=dx/dy ;
                   if((d<=new_d)&&(init==true))
                   {
                        ax+=(x-xi)*x ;
                        bx+=(y-yi)*x ;
//                        printf(" --- line-b x=%d y=%d ax=%f bx=%f  \n",x,y,ax,bx);
                   }
               }
           }
        }


        b=bx/ax;                 
        
        a=yi-b*xi;

        printf("--- c-point xi=%f,yi=%f \n",xi,yi);
        printf(" ---- linx y= %f+%fx \n", a,b);

                  
        for(int y=0;y !=height; ++y)
        {
           for(int x=0;x !=width; ++x)
           {
               if(255==data[y*step+x])
               {
                   dx=abs(b*y-x+a);
                   dy=sqrt(b*b+1);
                   d=dx/dy ;
                   if((d<=new_d)&&(init==true))
                   {
                       dx=abs(b*y-x+a);
                       dy=sqrt(b*b+1);
                       d+=dx/dy ;
 //                      printf("--- x=%d,y=%d  distance=%g \n",x,y,d);
                   }
               }
            }
 
        }

        di=d/num;

        printf("--- di=%f  \n",di);
        
        double si=0,si2=0;
 
        for(int y=0;y !=height; ++y)
        {
           for(int x=0;x !=width; ++x)
           {
               if(255==data[y*step+x])
               {   
                   dx=abs(b*y-x+a);
                   dy=sqrt(b*b+1);
                   d=dx/dy ;
                   if((d<=new_d)&&(init==true))
                   {
                   dx=abs(b*y-x+a);
                   dy=sqrt(b*b+1);
                   d=dx/dy ;
                   si+=(d-di)*(d-di); 
                   }
               }
           }

        }

       si2=si/(num-1);

       printf("--- si=%g  si2=%g  \n",si,si2);



       new_d=sqrt(si2);

       printf("--- new_d %g \n",new_d);

       init=false;        

       
       }
  
       for(int y=0;y !=height; y++)
        {
           for(int x=0;x !=width; x++)
           {
               if(y==(int(a+b*x)))
               {
                   data[y*step+x]=255;
               }
            }
        }


       


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
