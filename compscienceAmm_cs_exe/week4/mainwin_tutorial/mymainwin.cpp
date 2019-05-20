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
	mB0 = new QPushButton("next");
	mB1 = new QPushButton("prev");

	mHbox->addWidget(mB1);
	mHbox->addWidget(mB0);
	mVbox->addWidget(mWin);
	mVbox->addLayout(mHbox);
	mTop->setLayout(mVbox);

	connect(mB0,SIGNAL(clicked()),mWin,SLOT(nextImage()));
	connect(mB1,SIGNAL(clicked()),mWin,SLOT(prevImage()));

	connect(mB0,SIGNAL(clicked()),this,SLOT(showResolution()));
	connect(mB1,SIGNAL(clicked()),this,SLOT(showResolution()));

	connect(loadAction,SIGNAL(triggered()),this,SLOT(load()));
	connect(exitAction,SIGNAL(triggered()),qApp,SLOT(quit()));

	mFileMenu = menuBar()->addMenu("&File");
	mFileMenu->addAction(loadAction);
	mFileMenu->addSeparator();
	mFileMenu->addAction(exitAction);

	setCentralWidget(mTop);
	mBar = statusBar();

}

void MyMainWin::load()
{
	QStringList name = QFileDialog::getOpenFileNames(this, "test",".","*.jpg");
	mWin->setList(name);
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
