#include "myobject.h"
#include <iostream>

void MyObject::my_print() {
	std::cout << "hoge" << std::endl;
}

void MyObject::my_print2() {
	std::cout << "hoge2" << std::endl;
}

void MyObject::my_print3() {
	std::cout << "Now Japanese Unavailable" << std::endl;
}

void MyObject::quit() {
	std::cout << "Quit" << std::endl;
	exit(0);
}
