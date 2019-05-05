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
		y = x * 4;
	else if (x < 192)
		y = (x-192) * (-2);
	else
		y = (x-192) * 4;
	
	return y;
}

// curve4
int tonecurve4(int x) {
	double y;
	y = (x-127.5) * (x - 127.5) * 255 / (127.5 * 127.5);
	if((int)y > 255) return 255;
	else if((int)y < 0) return 0;
	else return (int)y;
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
	int f1, f2;
	int (*curves[4])(int) = { tonecurve1, tonecurve2, tonecurve3, tonecurve4 };
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
	if(argc < 9) {
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

	// f1 for selected area, f2 for left area
	f1 = atoi(argv[7]);
	f2 = atoi(argv[8]);

	if(f1 < 0 || f1 > 3 || f2 < 0 || f2 > 3) {
		printf("curve range error\n");
		exit(-1);
	}
	// apply tone curve to left area
	for(i=0;i<height;i++) {
		for(j=0;j<width;j++) {
			pixel = gdImageGetPixel(im,j,i);
			r = curves[f2](gdImageRed(im,pixel));
			g = curves[f2](gdImageGreen(im,pixel));
			b = curves[f2](gdImageBlue(im,pixel));
			color = gdImageColorExact(im_new,r,g,b);
			gdImageSetPixel(im_new,j,i,color);
		}
	}
	// apply tone curve to selected area
	for(i=modY;i<modY + modH;i++) {
		for(j=modX;j<modX + modW;j++) {
			pixel = gdImageGetPixel(im,j,i);

			// get filtered value by tone curve above
			r = curves[f1](gdImageRed(im,pixel));
			g = curves[f1](gdImageGreen(im,pixel));
			b = curves[f1](gdImageBlue(im,pixel));

			color = gdImageColorExact(im_new,r,g,b);

			gdImageSetPixel(im_new,j,i,color);
		}
	}

	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}

