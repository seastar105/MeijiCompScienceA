#include "mywindow.h"

MyWindow::MyWindow(QWidget *parent) : QWidget(parent) {
	setMinimumSize(qApp->desktop()->size()/2);
}

void MyWindow::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawImage(0,0,mImage);
}

void MyWindow::setFileName(QString str) 
{
	curFile = str;
	load();
}

void MyWindow::load()
{
	if(!curFile.isEmpty()) {
		if(QFile::exists(curFile)) {
			cv::Mat m = cv::imread(curFile.toStdString().c_str(),1);
			QImage im(m.data,m.cols,m.rows,QImage::Format_RGB888);
			im = im.rgbSwapped();
			int sWidth, sHeight;
			sWidth = mWidth = im.width(); sHeight = mHeight = im.height();
			if(sWidth > qApp->desktop()->width()/2) sWidth = qApp->desktop()->width()/2;
			if(sHeight > qApp->desktop()->height()/2) sHeight = qApp->desktop()->height()/2;
			mImage = im.scaled(sWidth,sHeight,Qt::KeepAspectRatio);
			setMinimumSize(mImage.size());
			update();
		}
	}
}

