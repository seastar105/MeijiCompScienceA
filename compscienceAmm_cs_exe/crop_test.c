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
	gdRect rect = {0,0,width/2,height/2};
//	rect.x = 0; rect.y = 0; rect.width = width/2; rect.height = height/2;
	im_new = gdImageCrop(im,&rect);
	gdImageJpeg(im_new,out,-1);
	
	fclose(in);
	fclose(out);

	return 0;


}
