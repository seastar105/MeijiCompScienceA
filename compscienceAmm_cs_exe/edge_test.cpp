#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	Mat im = imread(argv[1],1);
	Mat dst;//(im.rows,im.cols,CV_8UC3);
	Canny(im,dst,100,300);
	cout << dst.type() << endl;
	imwrite(argv[2],dst);
	return 0;
}
