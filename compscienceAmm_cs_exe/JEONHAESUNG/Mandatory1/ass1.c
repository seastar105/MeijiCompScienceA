#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(const int argc, const char *argv[]) {
    FILE *out, *in;
    gdImagePtr im,im_pad,im_new;
    int width, height, i, j, k, l, pixel, color,r,g,b;
	int w;
	int sum_r, sum_g, sum_b;
	int csum_r, csum_g, csum_b;
	double r_a, g_a, b_a;
	double divide;
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
	if(argc<3) {
		printf("Length of Filter is missing!\n");
		exit(-1);
	}
	w = atoi(argv[3]);
	divide = 1.0 / (2*w + 1) / (2*w + 1);
	im = gdImageCreateFromJpeg(in);

	width = gdImageSX(im);
	height = gdImageSY(im);

	im_pad = gdImageCreateTrueColor(width+2*w,height+2*w);
	gdImageCopy(im_pad,im,w,w,0,0,width,height);
	im_new = gdImageCreateTrueColor(width,height);

	for(j=w;j<height+w;j++) {
		for(i=w;i<width+w;i++) {
			sum_r = sum_g = sum_b = 0;
			for(l=-w;l<=w;l++) {
				for(k=-w;k<=w;k++) {
					pixel = gdImageGetPixel(im_pad,i+k,j+l);
					sum_r += gdImageRed(im_pad,pixel);
					sum_g += gdImageGreen(im_pad,pixel);
					sum_b += gdImageBlue(im_pad,pixel);
				}
			}
			r_a = sum_r * divide; g_a = sum_g * divide; b_a = sum_b * divide;
			color = gdImageColorExact(im_new,(int)r_a,(int)g_a,(int)b_a);
			gdImageSetPixel(im_new,i-w,j-w,color);
		}
	}
	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}
