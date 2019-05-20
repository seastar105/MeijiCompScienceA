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
 // cvtColor(src,src,CV_BGR2GRAY);
  bilateralFilter(src,dst,5,200,150);

  imwrite(argv[2],dst);
  return 0;
}
