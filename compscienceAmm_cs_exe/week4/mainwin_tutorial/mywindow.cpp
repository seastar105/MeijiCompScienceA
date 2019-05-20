#include "mywindow.h"
MyWindow::MyWindow(QWidget *parent) : QWidget(parent) {
	setMinimumSize(qApp->desktop()->size()/2);
}

void MyWindow::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawImage(0,0,mImage);
}

void MyWindow::setList(QStringList strs) 
{
	mList = strs;
	mSize = strs.size();
	mIdx = 0;
	load();
}

void MyWindow::load()
{
	QString str=mList[mIdx];
	if(!str.isEmpty()) {
		if(QFile::exists(str)) {
			QImage im = QImage(str);
			mWidth = im.width(); mHeight = im.height();
			mImage = im.scaled(qApp->desktop()->width()/2,
					qApp->desktop()->height()/2,Qt::KeepAspectRatio);
			setMinimumSize(mImage.size());
			update();
		}
	}
}

void MyWindow::nextImage() 
{
	mIdx++;
	if(mIdx == mSize)
		mIdx = 0;
	load();
}

void MyWindow::prevImage()
{
	if(mIdx == 0)
		mIdx = mSize-1;
	else
		mIdx--;
	load();
}
