#include "myobject.h"

int main(int argc, char **argv) {
	QApplication app(argc,argv);
	QWidget w;
	QHBoxLayout l1,l2;
	QVBoxLayout v;
	QPushButton button1("Hello World!");
	QPushButton button2("Bye World!");
	QPushButton button3("Push");
	QPushButton button4("Quit");
	MyObject m0,m1;

	l1.addWidget(&button1);
	l1.addWidget(&button2);
	l2.addWidget(&button3);
	l2.addWidget(&button4);
	QObject::connect(&button1,SIGNAL(clicked()),&m0,SLOT(my_print()));
	QObject::connect(&button2,SIGNAL(clicked()),&m1,SLOT(my_print2()));
	QObject::connect(&button3,SIGNAL(clicked()),&m1,SLOT(my_print3()));
	QObject::connect(&button4,SIGNAL(clicked()),&m1,SLOT(quit()));


	v.addLayout(&l1);
	v.addLayout(&l2);
	w.setLayout(&v);
	w.show();

	return app.exec();
}
