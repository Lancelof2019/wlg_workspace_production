#include "../headerspace/WatershedAlg.h"
using namespace cv;
void WatershedAlg::processImage(Mat image,Mat duplImage)  {


       image = makeImageGrayScale(image);

       vector< vector<int> > erosionTargetThree{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, };
       image = erosion(image, erosionTargetThree);
       image = thresholdMeasure(image);
       image=antiInverseImage(image);
       vector<Point> markers;
       image = distanceTransform(image,markers);
       Mat distanceImage = image;

       if(markers.size() == 0) {
            for(int i = 0; i < image.rows; i++) {
                for(int j = 0; j < image.cols; j++) {
                    if((int) distanceImage.at<uchar>(i,j) != 0) {
                        markers.push_back(Point(i, j));
                        image.at<uchar>(i, j) = 25;
           
                    }
                }
            }
        }


        image = watershed(image, markers,duplImage);

    }
