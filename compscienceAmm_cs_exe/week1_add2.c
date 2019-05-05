#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define THRESHOLD 50000000
#define BUF_MAX 256

void makeGrayScale(gdImagePtr im) {
	int i, j, width, height, r, g, b, L, pixel, color;
	double temp;
	width = gdImageSX(im);
	height = gdImageSY(im);
	for(i=0;i<width;i++) {
		for(j=0;j<height;j++) {
			pixel = gdImageGetPixel(im,i,j);

			r = gdImageRed(im,pixel);
			g = gdImageGreen(im,pixel);
			b = gdImageBlue(im,pixel);

			temp = 0.299 * r + 0.587 * g + 0.114 * b;
			L = (int)temp;

			color = gdImageColorExact(im,L,L,L);

			gdImageSetPixel(im,i,j,color);
		}
	}
	
}

int main(const int argc, const char *argv[]) {
	FILE *in, *t, *out;
	gdImagePtr imI, imT, imO;
	int width, height, twidth, theight;
	int i, j, color, r, g, b, pixelI, pixelT;
	int thr;
	int x,y;
	int min_SSD, minX,minY;
	int SSD,diff;
	int flag = 0,cnt = 0;
	char filename[BUF_MAX];
	if(argv[1] == NULL || argv[2]==NULL || !strcmp(argv[1],argv[3]) || !strcmp(argv[2],argv[3])) {
		printf("argument error\n");
		exit(-1);
	}
	if((in=fopen(argv[1],"r")) == NULL) {
		printf("file open error for %s\n",argv[1]);
		exit(-1);
	}
	if((t=fopen(argv[2],"r")) == NULL) {
		printf("file open error for %s\n",argv[2]);
		exit(-1);
	}
/*	if((out=fopen(argv[3],"w")) == NULL) {
		printf("file open error for %s\n",argv[3]);
		exit(-1);
	}*/
	const char *prefix = argv[3];
	if(argc > 4) thr = atoi(argv[4]);
	else thr = THRESHOLD;

	imI = gdImageCreateFromJpeg(in);
	width = gdImageSX(imI);
	height = gdImageSY(imI);
	
	imT = gdImageCreateFromJpeg(t);
	twidth = gdImageSX(imT);
	theight = gdImageSY(imT);

	if(width < twidth || height < theight) {
		printf("Range error\n");
		exit(-1);
	}

	// make input and template grayscale
//	makeGrayScale(imI);
//	makeGrayScale(imT);

	min_SSD = INT_MAX;
	minX = 0;
	minY = 0;
	// get difference from all channels
	for(y = 0;y<=height-theight;y++) {
		for(x = 0;x<=width-twidth;x++) {
			SSD = 0;
			for(j=0;j<theight;j++) {
				for(i=0;i<twidth;i++) {
					pixelI = gdImageGetPixel(imI,x+i,y+j);
					pixelT = gdImageGetPixel(imT,i,j);
					diff = gdImageRed(imT,pixelT) - gdImageRed(imI,pixelI);
					SSD += diff * diff;
					diff = gdImageBlue(imT,pixelT) - gdImageBlue(imI,pixelI);
					SSD += diff * diff;
					diff = gdImageGreen(imT,pixelT) - gdImageGreen(imI,pixelI);
					SSD += diff * diff;
					if(SSD > thr) break;
				}
				if(SSD > thr) break;
			}
			if(SSD <= thr) {
				flag = 1;
				gdRect crop = {x,y,twidth,theight};
				imO = gdImageCrop(imI,&crop);
				sprintf(filename, "%s%04d.JPG",prefix,cnt++);
				if((out=fopen(filename,"w")) == NULL) {
					printf("file open error for %s\n",filename);
					exit(-1);
				}
				gdImageJpeg(imO,out,-1);
				fclose(out);
			}
		}
	}
	if(!flag) {
		printf("Not Founded!\n");
		exit(-1);
	}

	fclose(in);
	fclose(t);

	return 0;

}
