#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*************************************************
 * make own tone curve, except identity function *
 * I'll use funtion which is similar to example  *
 * function in lecture note.                     *
 * **********************************************/

int tonecurve(int x) {
	int y;
	if(x < 64)
		y = 4 * x;
	else
		y = 255;
	return  y;
}

int tonecurve1(int x) {
	int y;
	if(x < 128)
		y = 2 * x;
	else 
		y = 255;
	return y;
}

int main(const int argc, const char *argv[]) {
	FILE *out, *in;
	gdImagePtr im,im_new;
	int width, height, i, j, color, r,g,b,pixel;

	if(argv[1] == NULL || argv[2]==NULL || !strcmp(argv[1],argv[2])) {
		printf("argument error\n");
		exit(-1);
	}
	if((in=fopen(argv[1],"r")) == NULL) {
		printf("file open error for %s\n",argv[1]);
		exit(-1);
	}
	if((out=fopen(argv[2],"w")) == NULL) {
		printf("file open error for %s\n",argv[2]);
		exit(-1);
	}

	im = gdImageCreateFromJpeg(in);
	width = gdImageSX(im);
	height = gdImageSY(im);

	im_new = gdImageCreateTrueColor(width,height);

	for(i=0;i<width;i++) {
		for(j=0;j<height;j++) {
			pixel = gdImageGetPixel(im,i,j);

			// get filtered value by tone curve above
			r = tonecurve(gdImageRed(im,pixel));
			g = tonecurve(gdImageGreen(im,pixel));
			b = tonecurve(gdImageBlue(im,pixel));

			color = gdImageColorExact(im_new,r,g,b);

			gdImageSetPixel(im_new,i,j,color);
		}
	}

	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}

