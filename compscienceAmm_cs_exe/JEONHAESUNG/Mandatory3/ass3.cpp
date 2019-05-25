#include "mymainwin.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	MyMainWin m(argv[1]);
	m.show();
	return app.exec();
}
