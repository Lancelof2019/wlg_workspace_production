#include "../headerspace/WatershedAlg.h"
#include <queue>
#include <cstdlib> 
#include <vector>
#define NUMSIZE 8
#define NSIZE 4
#define PNMTHR 35
//#include "/opt/project_space/test_mnist/HPC_comb_code/MA/Watershed_Algorithm/functionspace/removesmallregion.cpp"
using namespace cv;

/*bool comp(Point a,Point b){
      if(a.x!=b.x){
         return a.x<b.x;
     // else if(a.x==b.x)
      }
      return a.y<b.y;

}
*/
Mat WatershedAlg::distanceTransform(Mat image, vector<Point> &markers) {
        vector< vector<double> > distanceMap(image.rows, vector<double>(image.cols, 0));
        Mat markerImage(image.rows, image.cols, CV_8UC3, Scalar::all(0));

    /**************************/
       //cv::Mat dsrcImg=cv::imread("/opt/project_space/test_mnist/HPC_comb_code/watershed_demo/energy/12_energy_map.png");
       // Mat testImg=image;
        queue<int> qx, qy;
        int dx[NUMSIZE]={-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[NUMSIZE]={0, 0, -1, 1, -1,  1, 1, -1};
        //int numthresh=25;
        int pixelThreshold=55;
        //new added part:
        vector<bool>checkVisted;
        vector<Point>zeroEdge;
       /* string filename1="/opt/test1.txt";
        string filename2="/opt/test2.txt";
        string filename3="/opt/test3.txt";
        string filename27="/opt/test27.txt";
       // string filename4="/opt/test4.txt";
        string filename5="/opt/test5.txt";
        ofstream fout1(filename1);
        ofstream fout2(filename2);
        ofstream fout3(filename3);

        ofstream fout27(filename27);
       // ofstream fout4(filename4);
        ofstream fout5(filename5);
        */
        vector< vector<bool> > visited(image.rows, vector<bool>(image.cols, false));


        

//******It makes the nearest piexel along 0 point sohow up and become the edge pixel
        for(int i = 0; i < image.rows; i++) {
            for(int j = 0; j < image.cols; j++) {
                
                if(image.at<uchar>(i, j) != ZERO) {
                    continue;
                }
                //this is the part that sepreated from the edge
                
                for(int h = 0; h < NSIZE; h++) {
                    int nextX = i + dx[h];
                    int nextY = j + dy[h];

                    if( nextX < 0 || nextY < 0 || nextX >= image.rows || nextY >= image.cols ) {
                        continue;
                    }

                    if( !visited[nextX][nextY] && (int) image.at<uchar>(nextX, nextY) == ONE ) {
                        visited[nextX][nextY] = true;
                        image.at<uchar>(nextX, nextY) = pixelThreshold;
                        qx.push(nextX);
                        qy.push(nextY);
                       //the neighuours are all points who have 75 or 0 
                       //they are the points who are edges
                       checkVisted.push_back(visited[nextX][nextY]);

                    }
                }
                   /*if(checkVisted[0]||checkVisted[1]||checkVisted[2]||checkVisted[3]){
 

                   zeroEdge.push_back(Point(i,j));
                   checkVisted.clear();
                  }
                  */
         
             

        }
        
     }
    //******It makes the nearest piexel along 0 point sohow up and become the edge pixel
    
//edge is equal to 50
        int maxVal=0;
        //**Magic part**//
        while(!qx.empty()) {
            int crtX = qx.front(); qx.pop();
            int crtY = qy.front(); qy.pop();

            bool isBigger = true;//check
            //#pragma omp parallel for
            for(int h = 0; h < NUMSIZE; h++) {
                int nextX = crtX + dx[h];
                int nextY = crtY + dy[h];

                if( nextX < 0 || nextY < 0 || nextX >= image.rows || nextY >= image.cols || (int) image.at<uchar>(nextX, nextY) == ZERO ) {
                    continue;
                }

                if( (int) image.at<uchar>(crtX, crtY) <= (int) image.at<uchar>(nextX, nextY)) {
                    isBigger = false;
                    
                }
                //pick the max local value of some regions

                if( (int) image.at<uchar>(crtX, crtY) +1< (int) image.at<uchar>(nextX, nextY) ) {
                    visited[nextX][nextY] = true;
                    image.at<uchar>(nextX, nextY) = (uchar) min((image.at<uchar>(crtX, crtY)+1), 254);
                    
                    //to get max value for difference between max value image and image
                    if(maxVal<=(int)image.at<uchar>(nextX, nextY)){
                       maxVal=(int)image.at<uchar>(nextX, nextY);
                       
                    }
                     //to get max value for difference between max value image and image
                    qx.push(nextX);
                    qy.push(nextY);
                  //  fout4<<"the postion of push point: "<<Point(nextX,nextY)<<"The value of image: "<<((int)image.at<uchar>(nextX,nextY))<<",";
         

                }
                //fout4<<endl;
            }
           //find the max value in local area
            if(isBigger) {
                markers.push_back(Point(crtX, crtY));
               // cout<<"******************Potion and values******************"<<endl;
             //   cout<<Point(crtX, crtY)<<endl;
           
                
               // cout<<(int)image.at<uchar>(crtX,crtY)<<endl;
               // cout<<"The other parts of the value in position ctrx and ctry "<<endl;
               
                for(int h = 0; h < NUMSIZE; h++) {
                   int temp_nextX = crtX + dx[h];
                   int temp_nextY = crtY + dy[h];
                   if( temp_nextX < 0 || temp_nextY < 0 || temp_nextX >= image.rows || temp_nextY >= image.cols || (int) image.at<uchar>(temp_nextX, temp_nextY) == ZERO ) {
                   continue;
                }
                //  cout<<"The value of "<<"["<<temp_nextX<<","<<temp_nextY<<"]:"<<(int)image.at<uchar>(temp_nextX,temp_nextY)<<endl;
                }
         
                            

            }
        
        }

/*
            for(int i = 0; i < image.rows; i++) {
          for(int j = 0; j < image.cols; j++) {

          fout3<<((int)image.at<uchar>(i,j))<<",";
             

        }
         fout3<<endl;
     }
       fout3.close();
       */
   //############################mark the small regions###################################
  // Mat Rimg;
   pixelThreshold=pixelThreshold-1;
   
  // image.copyTo(Rimg);
   int pnumThrshold=PNMTHR;//the bound of removing holes
   int handlingType=0;
   int neighbourType=0;
   //removesmallregion(image,pnumThrshold,  handlingType, neighbourType, pixelThreshold);
   removeholesopt(image,pnumThrshold,  handlingType, neighbourType, pixelThreshold);


  
/*
        Mat vlogImg;
        image.copyTo(vlogImg);
        int x_coordi=0;
        int y_coordi=0;
       
        for(vector<Point>::iterator it=markers.begin();it<markers.end();it++){
         x_coordi=(*it).x;
         y_coordi=(*it).y;
         vlogImg.at<uchar>(x_coordi,y_coordi)=255;
        }

       // imshow("distanceTransformation",image);
       // cv::waitKey(0);
       // imshow("With seeds",vlogImg);
       // cv::waitKey(0);
*/


       return image;
    }
