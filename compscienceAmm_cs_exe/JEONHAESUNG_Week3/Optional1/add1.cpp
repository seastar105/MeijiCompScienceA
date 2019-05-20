#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>
#include<chrono>
#define EPOCH 100
using namespace std;
using namespace cv;

int main(int argc, const char**argv) {
  int x,y;
  int height, width;
  FILE *in;
  double sum;
  if((in=fopen(argv[1],"r")) == NULL) {
    cout << "file open error " << argv[1] << endl;
    exit(-1);
  }
  fclose(in);
  Mat m = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  sum = 0;
  height = m.rows;
  width = m.cols;
  for(int i=0;i<EPOCH;i++) {
	  chrono::system_clock::time_point start = chrono::system_clock::now();
	  for(y=0;y<height;y++) {
		for(x=0;x<width;x++) {
		  Vec3b bgr = m.at<Vec3b>(y,x);
		}
	  }
	  chrono::duration<double> sec = chrono::system_clock::now() - start;
	  sum += sec.count();
  }
  printf("%lf sec when using at\n",sum/EPOCH);
  sum = 0;
  for(int i=0;i<EPOCH;i++) {
	  chrono::system_clock::time_point start = chrono::system_clock::now();
	  for(y=0;y<height;y++) {
		Vec3b *p = m.ptr<Vec3b>(y);
		for(x=0;x<width;x++) {
		  Vec3b bgr = p[x];
		}
	  }
	  chrono::duration<double> sec = chrono::system_clock::now() - start;
	  sum += sec.count();
  }
  printf("%lf sec when using pointer\n",sum/EPOCH);
  return 0;
}
