#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int bilinear_sum(double x,double y, gdImagePtr im) {
    double r,g,b;
    int pixel;
    int fx,fy;
    r = g = b = 0.0;
    fx = (int)floor(x);
    fy = (int)floor(y);

    pixel = gdImageGetPixel(im,fx,fy);
    r += (fx+1-x)*(fy+1-y)*gdImageRed(im,pixel);
    g += (fx+1-x)*(fy+1-y)*gdImageGreen(im,pixel);
    b += (fx+1-x)*(fy+1-y)*gdImageBlue(im,pixel);

    pixel = gdImageGetPixel(im,fx,fy+1);
    r += (fx+1-x)*(y-fy)*gdImageRed(im,pixel);
    g += (fx+1-x)*(y-fy)*gdImageGreen(im,pixel);
    b += (fx+1-x)*(y-fy)*gdImageBlue(im,pixel);

    pixel = gdImageGetPixel(im,fx+1,fy);
    r += (x-fx)*(fy+1-y)*gdImageRed(im,pixel);
    g += (x-fx)*(fy+1-y)*gdImageGreen(im,pixel);
    b += (x-fx)*(fy+1-y)*gdImageBlue(im,pixel);

    pixel = gdImageGetPixel(im,fx+1,fy+1);
    r += (fx-x)*(fy-y)*gdImageRed(im,pixel);
    g += (fx-x)*(fy-y)*gdImageGreen(im,pixel);
    b += (fx-x)*(fy-y)*gdImageBlue(im,pixel);

    return 256*256*(int)r + 256*(int)g + (int)b;
}


int valid(int x, int y, int width, int height) {
	if( x<0 || x >= width) return 0;
	else if ( y<0 || y >= height) return 0;
	return 1;
}

int main(const int argc, const char *argv[]) {
    FILE *out, *in;
    gdImagePtr im,im_new;
    int width, height, i, j, pixel, color,r,g,b;
	int new_width, new_height;
	int x,y,xd,yd;
	double theta, beta;
	int sum;

    if(argv[1]==NULL||argv[2]==NULL||!strcmp(argv[1],argv[2])){
         printf("argument error\n");
         exit(-1);
    }
    //第一引数で指定されたファイルを読み出し用にオープン
    if((in=fopen(argv[1],"r"))==NULL){
         printf("file open error for %s\n",argv[1]);
         exit(-1);
    }
    //第二引数で指定されたファイルを書き出し用にオープン
    if((out=fopen(argv[2],"wb"))==NULL){
         printf("file open error for %s\n",argv[2]);
         exit(-1);
    }
	if(argc < 3) {
		printf("missing theta, input correctly\n");
		exit(-1);
	}
	theta = atof(argv[3]);
	beta = atan(theta * M_PI / 180.0);
	im = gdImageCreateFromJpeg(in);

	width = gdImageSX(im);
	height = gdImageSY(im);
	new_width = width;
	new_height = height + beta * width;
	im_new = gdImageCreateTrueColor(new_width,new_height);
	for(yd=0;yd<new_height;yd++) {
		for(xd=0;xd<new_width;xd++) {
			sum = bilinear_sum((double)xd, -beta * (double)xd + (double)yd, im);
			r = sum / (256 * 256);
			g = (sum / 256) %  256;
			b = sum % 256;
			color = gdImageColorExact(im_new,r,g,b);
			gdImageSetPixel(im_new,xd,yd,color);
		}
	}

	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}
