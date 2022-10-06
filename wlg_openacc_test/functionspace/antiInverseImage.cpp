
#include "../headerspace/WatershedAlg.h"
using namespace cv;

Mat WatershedAlg::antiInverseImage(Mat image) {
    auto *startImg=image.data;
    int imgrows=image.rows;
    int imgcols=image.cols;
    #pragma acc enter data copyin(startImg[:imgrows*imgcols])
    #pragma acc parallel loop default(present)
        for(int i = 0; i < imgrows*imgcols; i++) {
                  if((int)startImg[i]==ZERO){
					  startImg[i]=ZERO;
				  }
				  else{
				      startImg[i]=ONE;
				  }
		  }
	#pragma acc exit data copyout(startImg[:imgrows*imgcols])

        return image;
    }
