#include<QtGui>
class MyObject : public QObject
{
	Q_OBJECT
	public:
	signals:
	public slots:
		void my_print();
		void my_print2();
		void my_print3();
		void quit();
	private:

};
