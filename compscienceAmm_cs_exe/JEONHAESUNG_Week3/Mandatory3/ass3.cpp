#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, const char**argv) {
  Mat src=imread(argv[1],CV_LOAD_IMAGE_COLOR);
  // Mat src=imread(argv[1],1);
  Mat dst(src.rows,src.cols,CV_8UC3);
  Mat mat=(Mat_<float>(2,3)<<-1,0,src.cols,0,1,0);

  warpAffine(src,dst,mat,dst.size());
  
  imwrite(argv[2],dst);
  return 0;
}
