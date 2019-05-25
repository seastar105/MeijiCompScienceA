#include "mymainwin.h"
#include <sstream>
MyMainWin::MyMainWin() {

}
MyMainWin::MyMainWin(const char *str) 
{
//	loadAction = new QAction("&Load", this);
//	saveAction = new QAction("&Save", this);
	exitAction = new QAction("&Exit", this);
	mWin = new MyWindow(str);
	mTop = new QWidget();
	mHbox1 = new QHBoxLayout();
	mHbox2 = new QHBoxLayout();
	mVbox = new QVBoxLayout();
	mB1 = new QPushButton("Gaussian");
	mB2 = new QPushButton("Edge");
	mB3 = new QPushButton("Mirror");
	mB4 = new QPushButton("Normal");

	mHbox1->addWidget(mB1);
	mHbox1->addWidget(mB2);
	mHbox2->addWidget(mB3);
	mHbox2->addWidget(mB4);
	mVbox->addWidget(mWin);
	mVbox->addLayout(mHbox1);
	mVbox->addLayout(mHbox2);
	mTop->setLayout(mVbox);

//	connect(loadAction,SIGNAL(triggered()),this,SLOT(load()));
	connect(exitAction,SIGNAL(triggered()),qApp,SLOT(quit()));
//	connect(loadAction,SIGNAL(triggered()),this,SLOT(showResolution()));
	connect(mB1,SIGNAL(clicked()),mWin,SLOT(Gaussian()));
	connect(mB2,SIGNAL(clicked()),mWin,SLOT(Edge()));
	connect(mB3,SIGNAL(clicked()),mWin,SLOT(Mirror()));
	connect(mB4,SIGNAL(clicked()),mWin,SLOT(Normal()));

	mFileMenu = menuBar()->addMenu("&File");
//	mFileMenu->addAction(loadAction);
//	mFileMenu->addSeparator();
	mFileMenu->addAction(exitAction);

	setCentralWidget(mTop);
	mBar = statusBar();
	mWin->start();
//	printf("Done?\n");
}

void MyMainWin::load()
{
	QString name = QFileDialog::getOpenFileName(this, "test",".","*.jpg");
	showResolution();
}

void MyMainWin::showResolution()
{
	std::stringstream ss;
	ss << "Resolution" << mWin->mWidth << "x" << mWin->mHeight;
	mBar->showMessage(ss.str().c_str());
}

/*void MyMainWin::save()
{
	QString name = QFileDialog::getSaveFileName(this, "test", ".", "");
}*/
