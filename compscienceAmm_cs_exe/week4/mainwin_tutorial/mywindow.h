#include<QtGui>
class MyWindow : public QWidget
{
	Q_OBJECT
	public:
		MyWindow(QWidget *parent=0);
		void setList(QStringList);
		int mWidth,mHeight;
	public slots:
		void nextImage();
		void prevImage();
	protected:
		void paintEvent(QPaintEvent*);
	private:
		void load();
		QImage mImage;
		QStringList mList;
		unsigned int mIdx;
		unsigned int mSize;
};
