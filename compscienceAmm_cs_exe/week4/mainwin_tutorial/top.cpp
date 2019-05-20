#include "mymainwin.h"

int main(int argc, char *argv[]) {
	QApplication app(argc,argv);
	MyMainWin m;
	m.show();
	return app.exec();
}
