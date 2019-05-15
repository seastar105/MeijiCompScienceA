#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>
#include<gd.h>

using namespace std;
using namespace cv;

int main(int argc, const char**argv) {
  gdImagePtr im;
  int r,g,b,x,y,pixel,width,height;
  FILE *out,*in;
  if((in=fopen(argv[1],"r")) == NULL) {
    cout << "file open error " << argv[1] << endl;
    exit(-1);
  }
  im = gdImageCreateFromJpeg(in);
  width = gdImageSX(im);
  height = gdImageSY(im);

  Mat m(height,width,CV_8UC3);
  Vec3b bgr;
  for(y=0;y<height;y++) {
    for(x=0;x<width;x++) {
      pixel = gdImageGetPixel(im,x,y);
      bgr[2] = gdImageRed(im,pixel);
      bgr[1] = gdImageGreen(im,pixel);
      bgr[0] = gdImageBlue(im,pixel);
      m.at<Vec3b>(y,x) = bgr;
    }
  }
  imwrite(argv[2],m);
  return 0;
}
