#include<QtGui>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

class MyWindow : public QWidget
{
	Q_OBJECT
	public:
		MyWindow(QWidget *parent=0);
		MyWindow(const char *str, QWidget *parent=0);
		void setFileName(QString);
		int mWidth,mHeight;
	public slots:
		void start();
	private slots:
		void Gaussian();
		void Edge();
		void Mirror();
		void Normal();
		void drawVideo();
	protected:
		void paintEvent(QPaintEvent*);
	private:
		void initialize();
		QImage mImage;
		QTimer mTimer;
		VideoCapture mCap;
		Mat mirrorMat,out;
		enum IPType {normal,gauss,edge,mirror} curType;
		QVector<QRgb> sColorTable;
};
