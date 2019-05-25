#include <QtGui>
#include "mywindow.h"
class MyMainWin : public QMainWindow
{
	Q_OBJECT
	public:
		MyMainWin();
		MyMainWin(const char *str);
	private slots:
		void load();
//		void save();
		void showResolution();
	private:
		QAction *loadAction;
//		QAction *saveAction;
		QAction *exitAction;
		QMenu *mFileMenu;
		QStatusBar *mBar;
		MyWindow *mWin;
		QHBoxLayout *mHbox1,*mHbox2;
		QVBoxLayout *mVbox;
		QWidget *mTop;
		QPushButton *mB1, *mB2, *mB3, *mB4;		// mB123 are for IP, mB4 is for quit
};
