#include "../headerspace/WatershedAlg.h"
using namespace cv;
#define ZERO 0
 void WatershedAlg::removeholesopt(Mat& src,int pnumThrshold, int handlingType, int neighbourType,int pixelThreshold)  {
   
   
  int RemoveCount=0;       

    Mat Pointlabel(src.rows,src.cols, CV_8UC1,Scalar::all(255) );  
    
 
     auto *srcstart=src.data;
	 auto *plstart=Pointlabel.data;
	 int srcrows=src.rows;
	 int srccols=src.cols;
 
	#pragma acc enter data copyin(srcstart[:srcrows*srccols],plstart[:srcrows*srccols],pixelThreshold)
        #pragma acc parallel loop default(present)
		for(int i=0;i<srcrows*srccols;i++){
			
			if(srcstart[i]>pixelThreshold){
				plstart[i]=0;
			}
			
		}
	#pragma acc exit data copyout(srcstart[:srcrows*srccols],plstart[:srcrows*srccols])
	 
    vector<vector<Point>> contours;  
    vector<cv::Vec4i> hierarchy; 
    findContours(Pointlabel,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point()); 
    Mat imageContours=Mat::zeros(Pointlabel.size(),CV_8UC1);  
    Mat contouroutput=Mat::zeros(Pointlabel.size(),CV_8UC1);  
    Point p=Point(0,0);



   if (!contours.empty() && !hierarchy.empty()) 
	{
		vector<vector<Point> >::const_iterator itc = contours.begin();
	
		while (itc != contours.end()) 
		{
			
			cv::Rect countourposition = cv::boundingRect(Mat(*itc));//number of countourposition
			
			double area = contourArea(*itc);
			int countoury=countourposition.y;
			int countourx=countourposition.x;
			int countourybd=countourposition.y + countourposition.height;
			int countourxbd=countourposition.x + countourposition.width;
			
			if (area < pnumThrshold) 
			{
				
				#pragma acc enter data copyin(srcstart[:srcrows*srccols],pixelThreshold,plstart[:srcrows*srccols])
				#pragma acc parallel loop collapse(2) default(present)
				for (int i = countoury; i < countourybd; i++) 
				{
					
                 
					for (int j = countourx; j < countourxbd; j++) {
						if((int)plstart[i*countourxbd+j]==255){
						plstart[i*+countourxbd+j]=2;
					        srcstart[i*countourxbd+j]=pixelThreshold;
						
						}
						
					}
				}
				#pragma acc exit data copyout(srcstart[:srcrows*srccols],plstart[:srcrows*srccols])
				
	
			}//if
			itc++;
		}//while
	}//if

 }
