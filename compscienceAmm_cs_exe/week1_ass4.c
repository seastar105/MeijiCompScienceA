#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*************************************************
 * make own tone curve, except identity function *
 * I'll use funtion which is similar to example  *
 * function in lecture note.                     *
 * **********************************************/

// this curve makes image lighter
// 4 times value under 64 and change values over 63 into 255
int tonecurve1(int x) {
	int y;
	if(x < 64)
		y = 4 * x;
	else
		y = 255;
	return  y;
}

// curve2 takes values from 192 to 255 then discard other values to 0
int tonecurve2(int x) {
	int y;
	if(x < 192)
		y = 0;
	else
		y = (x-192)*4;

	return y;
}

// curve3 
int tonecurve3(int x) {
	int y;
	if(x<64)
		y = 0;
	else if (x < 192)
		y = (x - 64) * 2;
	else
		y = 255;
	
	return y;
}

int rangecheck(int start, int offset, int limit) {
	if (start < 0 || start >= limit) return 0;
	if (start + offset < 0 || start+offset >= limit) return 0;
	return 1;
}
int main(const int argc, const char *argv[]) {
	FILE *out, *in;
	gdImagePtr im,im_new;
	int width, height, i, j, color, r,g,b,pixel;
	int modX,modY,modW,modH;
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
	if(argc < 7) {
		printf("need more argument\n");
		exit(-1);
	}
	im = gdImageCreateFromJpeg(in);
	width = gdImageSX(im);
	height = gdImageSY(im);

	im_new = gdImageClone(im);

	modX = atoi(argv[3]);
	modY = atoi(argv[4]);
	modH = atoi(argv[5]);
	modW = atoi(argv[6]);

	if(!(rangecheck(modX,modW,width) && rangecheck(modY,modH,height))) {
		printf("range error\n");
		exit(-1);
	}
	for(i=modY;i<modY + modH;i++) {
		for(j=modX;j<modX + modW;j++) {
			pixel = gdImageGetPixel(im,j,i);

			// get filtered value by tone curve above
			r = tonecurve1(gdImageRed(im,pixel));
			g = tonecurve1(gdImageGreen(im,pixel));
			b = tonecurve1(gdImageBlue(im,pixel));

			color = gdImageColorExact(im_new,r,g,b);

			gdImageSetPixel(im_new,j,i,color);
		}
	}

	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}

