#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(const int argc, const char *argv[]) {
	FILE *out, *in;
	    gdImagePtr im,im_new;
	    int width, height, i, j, pixel, color,r,g,b;
		double temp;
		int L;
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
		
		// open input image
		im = gdImageCreateFromJpeg(in);
		width = gdImageSX(im);
		height = gdImageSY(im);

		// create output image
		im_new = gdImageCreateTrueColor(width,height);

		for(i=0;i<width;i++) {
			for(j=0;j<height;j++) {
				// get color from (i,j)
				pixel = gdImageGetPixel(im,i,j);

				// get RGB from pixel
				r = gdImageRed(im,pixel);
				g = gdImageGreen(im,pixel);
				b = gdImageBlue(im,pixel);

				// calculate L
				temp = 0.299 * r + 0.587 * g + 0.114 * b;
				L = (int)temp;

				// make grayscale color
				color = gdImageColorExact(im_new,L,L,L);

				// write color into new image's (i,j)
				gdImageSetPixel(im_new,i,j,color);
			}
		}

		gdImageJpeg(im_new,out,-1);

		fclose(in);
		fclose(out);

		return 0;
}
