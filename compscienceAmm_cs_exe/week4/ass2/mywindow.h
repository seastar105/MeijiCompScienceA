#include<QtGui>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
class MyWindow : public QWidget
{
	Q_OBJECT
	public:
		MyWindow(QWidget *parent=0);
		void setFileName(QString);
		int mWidth,mHeight;
	public slots:
	protected:
		void paintEvent(QPaintEvent*);
	private:
		void load();
		QImage mImage;
		QString curFile;
};
