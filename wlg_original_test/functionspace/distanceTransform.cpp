#include "../headerspace/WatershedAlg.h"
#include <queue>
#include <cstdlib> 
#include <vector>

#define NUMSIZE 8
#define NSIZE 4


using namespace cv;


Mat WatershedAlg::distanceTransform(Mat image, vector<Point> &markers) {
        vector< vector<double> > distanceMap(image.rows, vector<double>(image.cols, 0));
        Mat markerImage(image.rows, image.cols, CV_8UC3, Scalar::all(0));
        queue<int> qx, qy;
        int dx[NUMSIZE]={-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[NUMSIZE]={0, 0, -1, 1, -1,  1, 1, -1};
        int pixelThreshold=55;
        vector<bool>checkVisted;
        vector<Point>zeroEdge;
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
            
                    qx.push(nextX);
                    qy.push(nextY);
             

                }
        
            }
           //find the max value in local area
            if(isBigger) {
                markers.push_back(Point(crtX, crtY));
                for(int h = 0; h < NUMSIZE; h++) {
                   int temp_nextX = crtX + dx[h];
                   int temp_nextY = crtY + dy[h];
                   if( temp_nextX < 0 || temp_nextY < 0 || temp_nextX >= image.rows || temp_nextY >= image.cols || (int) image.at<uchar>(temp_nextX, temp_nextY) == ZERO ) {
                   continue;
                }
             }
         
                            

            }
        
        }

   //############################mark the small regions###################################

   pixelThreshold=pixelThreshold-1;
   int pnumThrshold=30;
   int handlingType=0;
   int neighbourType=0;
   removeholesopt(image,pnumThrshold,  handlingType, neighbourType, pixelThreshold);

   return image;
    }
