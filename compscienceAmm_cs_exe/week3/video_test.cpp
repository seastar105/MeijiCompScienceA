#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, const char **argv){
  VideoCapture video(1);
  bool isOpened = video.isOpened();
  namedWindow("test");
  Mat frame;
  while(1) {
    video >> frame;
    Mat dst(frame.rows,frame.cols,frame.type());
    Mat mat=(Mat_<float>(2,3)<<-1,0,frame.cols,0,1,0);
    //  GaussianBlur(frame,dst,Size(5,5),1.5,1.5);
    warpAffine(frame,dst,mat,dst.size());
    
    if(waitKey(30) == 'q')
      break;
    
    imshow("test",dst);
  }
  return 0;
}
