#include <QtGui>
#include "mywindow.h"
class MyMainWin : public QMainWindow
{
	Q_OBJECT
	public:
		MyMainWin();
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
		QPushButton *mB0, *mB1;
		MyWindow *mWin;
		QHBoxLayout *mHbox;
		QVBoxLayout *mVbox;
		QWidget *mTop;
};
