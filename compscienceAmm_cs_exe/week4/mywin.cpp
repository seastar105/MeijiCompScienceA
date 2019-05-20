#include "mywindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QWidget w;
	QPushButton b("button1");
	MyWindow mywin(argv[1]);
	QVBoxLayout l;
	l.addWidget(&mywin);
	l.addWidget(&b);
	w.setLayout(&l);
	w.show();
	return app.exec();
}
