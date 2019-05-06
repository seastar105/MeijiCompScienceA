#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void direct_transformation(int *x, int *y, double m) {

}

void inverse_transformation(int *x, int *y, double m) {

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
	int x_offset, y_offset;
	double slope_a, slope_b, m;

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
	if(argc < 4) {
		printf("missing a or b, input correctly\n");
		exit(-1);
	}
	slope_a = atof(argv[3]);
	slope_b = atof(argv[4]);
	m = -slope_a / slope_b;
	im = gdImageCreateFromJpeg(in);

	width = gdImageSX(im);
	height = gdImageSY(im);
	new_width = width + beta * height;
	new_height = height;
	im_new = gdImageCreateTrueColor(new_width,new_height);
	for(yd=0;yd<new_height;yd++) {
		for(xd=0;xd<new_width;xd++) {
			x = (int)floor((double)xd - beta * (double)yd + 0.5);
			y = (int)floor((double)yd + 0.5);
			if(!valid(x,y,width,height)) continue;
			pixel = gdImageGetPixel(im,x,y);
			r = gdImageRed(im,pixel);
			g = gdImageGreen(im,pixel);
			b = gdImageBlue(im,pixel);
			color = gdImageColorExact(im_new,r,g,b);
			gdImageSetPixel(im_new,xd,yd,color);
		}
	}

	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}
