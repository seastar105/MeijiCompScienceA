#include "mywindow.h"

void MyWindow::initialize() {
	Mat frame;
	curType = normal;
	mCap >> frame;
	QImage img(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
	mImage = img.rgbSwapped();
	mirrorMat = (Mat_<float>(2,3) << -1,0,frame.cols,0,1,0);
	sColorTable.resize(256);
	for(int i=0;i<256;i++) {
		sColorTable[i] = qRgb(i,i,i);
	}
	setMinimumSize(frame.cols, frame.rows);
}

MyWindow::MyWindow(const char *str, QWidget *parent) : QWidget(parent) {
	mCap = VideoCapture(str);
	initialize();
//	start();
}

MyWindow::MyWindow(QWidget *parent) : QWidget(parent) {
	mCap = VideoCapture(0);
	initialize();
}

void MyWindow::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawImage(0,0,mImage);
}

void MyWindow::drawVideo() {
	Mat frame,gray;
	mCap >> frame;
	switch(curType) {
		case normal: out = frame; break;
		case gauss:
					 GaussianBlur(frame,out,Size(7,7),2.5);break;
		case edge:
					 Canny(frame,out,50,150);
					 break;
		case mirror:
					 warpAffine(frame,out,mirrorMat,out.size());break;
		default:
					 break;
	}
	if(curType == edge) {
		QImage img((uchar*)out.data, out.cols, out.rows, QImage::Format_Indexed8);
		img.setColorTable(sColorTable);
		mImage = img;
	}
	else {
		QImage img(out.data, out.cols, out.rows, QImage::Format_RGB888);
		mImage = img.rgbSwapped();
	}
	update();
}


void MyWindow::start() {
	connect(&mTimer, SIGNAL(timeout()),this,SLOT(drawVideo()));
	mTimer.start(30);
}

void MyWindow::Gaussian() {
	curType = gauss;
}

void MyWindow::Edge() {
	curType = edge;
}

void MyWindow::Mirror() {
	curType = mirror;
}

void MyWindow::Normal() {
	curType = normal;
}
