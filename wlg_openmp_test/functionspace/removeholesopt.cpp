#include "../headerspace/WatershedAlg.h"
using namespace cv;
#define ZERO 0
 void WatershedAlg::removeholesopt(Mat& src,int pnumThrshold, int handlingType, int neighbourType,int pixelThreshold)  {
   
   
 //  cv::imshow("distanceTransform before remove holes",src);
 //  cv::waitKey(0);
    int RemoveCount=0;       
    //string filename33="/opt/test33.txt";
   // ofstream fout33(filename33);
 //   string filename34="/opt/test34.txt";
 //   ofstream fout34(filename34);
   //  string filename35="/opt/test35.txt";
  //  ofstream fout35(filename35);
   // vector<Point> NeihborPos;
    //queue<int> qx, qy;
    //vector<Point>pixbuffer;
    Mat Pointlabel(src.rows,src.cols, CV_8UC1,Scalar::all(255) );  
    
 
 
       // cout<<"removing holes start. "<<endl;  
        #pragma omp parallel for collapse(2)
        for(int i = 0; i < src.rows; ++i)  {    
           // uchar* iData = src.ptr<uchar>(i);  
           // uchar* iLabel = Pointlabel.ptr<uchar>(i);  
           
            for(int j = 0; j < src.cols; ++j) {    
                 if(src.at<uchar>(i,j)>pixelThreshold){
                 Pointlabel.at<uchar>(i,j)=0;
                 
               
                 }
               // if (iData[j] > pixelThreshold)    
              //  {    
                   // iLabel[j] = 3;   
              //  }    
            }    
        }  

//**

 //  imshow("prepration for contours",Pointlabel);
//
  //  cv::waitKey(0);

   // Mat dulpPointlabel;
    //Pointlabel.copyTo(dulpPointlabel);
    vector<vector<Point>> contours;  
    vector<cv::Vec4i> hierarchy; 
    findContours(Pointlabel,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point()); 
    Mat imageContours=Mat::zeros(Pointlabel.size(),CV_8UC1);  
    Mat contouroutput=Mat::zeros(Pointlabel.size(),CV_8UC1);  
    Point p=Point(0,0);



   if (!contours.empty() && !hierarchy.empty()) 
	{
		vector<vector<Point> >::const_iterator itc = contours.begin();
   // cout<<"The number of size of contours"<<contours.size()<<endl;
	
		while (itc != contours.end()) 
		{
			
			cv::Rect countourposition = cv::boundingRect(Mat(*itc));
			
			double area = contourArea(*itc);
		
			if (area < pnumThrshold) 
			{
				#pragma omp parallel for collapse(2)
				for (int i = countourposition.y; i < countourposition.y + countourposition.height; i++) 
				{
					//uchar *output_data = dst.ptr<uchar>(i);
         
					for (int j = countourposition.x; j < countourposition.x + countourposition.width; j++) 
					{
						
						if ((int)Pointlabel.at<uchar>(i,j) == 255) 
						{
							Pointlabel.at<uchar>(i,j) = 2;
              src.at<uchar>(i,j)=pixelThreshold;

						}
					}
				}
			}
			itc++;
		}
	}

/*
       for(int i=0;i<Pointlabel.rows;i++){
           for(int j=0;j<Pointlabel.cols;j++){
              fout34<<(int)Pointlabel.at<uchar>(i,j)<<",";
             

         }
           fout34<<endl;
      }
           fout34.close();

*/


/*


    for(int i=0;i<contours.size();i++)  
    {  
        //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数  
        for(int j=0;j<contours[i].size();j++)   
        {  
            //绘制出contours向量内所有的像素点  
           p=Point(contours[i][j].x,contours[i][j].y);  
            contouroutput.at<uchar>(p)=255;  
        }  
  
        //输出hierarchy向量内容  
       // char ch[256];  
       // sprintf(ch,"%d",i);  
       // string str=ch;  
       // cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<endl<<hierarchy[i]<<endl<<endl;  
  
        //绘制轮廓  
        drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);  
    }  
    
     
    imshow("Point of Contours :contouroutput",contouroutput);   //向量contours内保存的所有轮廓点集  
    waitKey(0);  
    imshow("Contours Image:imageContours",imageContours); //轮廓  
    waitKey(0); 




//**


*/

     /*  for(int i=0;i<dulpPointlabel.rows;i++){
           for(int j=0;j<dulpPointlabel.cols;j++){
              fout33<<(int)dulpPointlabel.at<uchar>(i,j)<<",";
             

         }
           fout33<<endl;
      }
           fout33.close();




       */

 }