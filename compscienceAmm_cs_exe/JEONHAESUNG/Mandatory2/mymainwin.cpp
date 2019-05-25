#include "mymainwin.h"
#include <sstream>

MyMainWin::MyMainWin() 
{
	loadAction = new QAction("&Load", this);
//	saveAction = new QAction("&Save", this);
	exitAction = new QAction("&Exit", this);
	
	mWin = new MyWindow();
	mTop = new QWidget();
	mHbox = new QHBoxLayout();
	mVbox = new QVBoxLayout();

	mVbox->addWidget(mWin);
	mVbox->addLayout(mHbox);
	mTop->setLayout(mVbox);

	connect(loadAction,SIGNAL(triggered()),this,SLOT(load()));
	connect(exitAction,SIGNAL(triggered()),qApp,SLOT(quit()));
	connect(loadAction,SIGNAL(triggered()),this,SLOT(showResolution()));

	mFileMenu = menuBar()->addMenu("&File");
	mFileMenu->addAction(loadAction);
	mFileMenu->addSeparator();
	mFileMenu->addAction(exitAction);

	setCentralWidget(mTop);
	mBar = statusBar();

}

void MyMainWin::load()
{
	QString name = QFileDialog::getOpenFileName(this, "test",".","*.jpg");
	mWin->setFileName(name);
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
