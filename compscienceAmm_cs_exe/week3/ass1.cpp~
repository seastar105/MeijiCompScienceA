#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, const char**argv) {
  const char *filename = "lena.jpg";
  Mat im;
  im=imread(filename, CV_LOAD_IMAGE_COLOR);
  imshow("TEST",im);
  imwrite("output.jpg",im);
  waitKey();
  return 0;
}
