#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>
#include<chrono>
#include<gd.h>

using namespace std;
using namespace cv;

int main(int argc, const char**argv) {
  gdImagePtr im;
  int r,g,b,x,y,pixel,width,height;
  FILE *out,*in;
  double sum;
  if((in=fopen(argv[1],"r")) == NULL) {
    cout << "file open error " << argv[1] << endl;
    exit(-1);
  }
  im = gdImageCreateFromJpeg(in);
  width = gdImageSX(im);
  height = gdImageSY(im);

  Mat m(height,width,CV_8UC3);
  Vec3b bgr;
  sum = 0;
  for(int i=0;i<5;i++) {
	  chrono::system_clock::time_point start = chrono::system_clock::now();
	  for(y=0;y<height;y++) {
		for(x=0;x<width;x++) {
		  pixel = gdImageGetPixel(im,x,y);
		  bgr[2] = gdImageRed(im,pixel);
		  bgr[1] = gdImageGreen(im,pixel);
		  bgr[0] = gdImageBlue(im,pixel);
		  m.at<Vec3b>(y,x) = bgr;
		}
	  }
	  chrono::duration<double> sec = chrono::system_clock::now() - start;
	  sum += sec.count();
  }
  printf("%lf sec for at\n",sum/5);
  sum = 0;
  for(int i=0;i<5;i++) {
	  chrono::system_clock::time_point start = chrono::system_clock::now();
	  for(y=0;y<height;y++) {
		Vec3b *p = m.ptr<Vec3b>(y);
		for(x=0;x<width;x++) {
		  pixel = gdImageGetPixel(im,x,y);
		  bgr[2] = gdImageRed(im,pixel);
		  bgr[1] = gdImageGreen(im,pixel);
		  bgr[0] = gdImageBlue(im,pixel);
		  p[x] = bgr;
		}
	  }
	  chrono::duration<double> sec = chrono::system_clock::now() - start;
	  sum += sec.count();
  }
  printf("%lf sec for pointer\n",sum/5);
  imwrite(argv[2],m);
  return 0;
}
