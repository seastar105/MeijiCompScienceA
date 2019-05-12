#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(const int argc, const char *argv[]) {
	FILE *out, *in;
	gdImagePtr im,im_new;
	int width, height, i, j, pixel, color,r,g,b;

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
	
	// read image from input
	im = gdImageCreateFromJpeg(in);

	// get width and height from input image
	width = gdImageSX(im);
	height = gdImageSY(im);

	// make new image for output
	im_new = gdImageCreateTrueColor(height, width);

	// write new image into im_new
	// (i,j) to (j,i)
	for (j=0;j<height;j++) {
		for(i=0;i<width;i++) {
			// get pixel from input
			pixel = gdImageGetPixel(im,i,j);

			r = gdImageRed(im,pixel);
			g = gdImageGreen(im,pixel);
			b = gdImageBlue(im,pixel);

			color = gdImageColorExact(im_new,r,g,b);

			gdImageSetPixel(im_new,j,width-1-i,color);
		}
	}
	gdImageJpeg(im_new,out,-1);
	
	fclose(in);
	fclose(out);

	return 0;


}
