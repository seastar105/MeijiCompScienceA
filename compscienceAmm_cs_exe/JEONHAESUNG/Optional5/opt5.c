#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

/*
   this program does alpha blending that mixing two input images
   if two inputs have different size, terminates the program
 */

int main(const int argc, const char *argv[]) {
    FILE *out, *in1, *in2;
	char filename[256];
    gdImagePtr im1,im2,im_new;
    int width, height, x, y, pixel1, pixel2, color,r,g,b;
	int i,n;
	double alpha;
    if(argv[1]==NULL||argv[2]==NULL||argv[3]==NULL||!strcmp(argv[1],argv[2])||!strcmp(argv[1],argv[3])||!strcmp(argv[2],argv[3])){
         printf("argument error\n");
         exit(-1);
    }
    //第一引数で指定されたファイルを読み出し用にオープン
    if((in1=fopen(argv[1],"r"))==NULL){
         printf("file open error for %s\n",argv[1]);
         exit(-1);
    }
    //第二引数で指定されたファイルを書き出し用にオープン
    if((in2=fopen(argv[2],"r"))==NULL){
         printf("file open error for %s\n",argv[2]);
         exit(-1);
    }
	if(argc < 5) {
		printf("missing alpha, input correctly\n");
		exit(-1);
	}
	const char *prefix = argv[3];
	n = atoi(argv[4]);
	im1 = gdImageCreateFromJpeg(in1);
	im2 = gdImageCreateFromJpeg(in2);

	if(gdImageSX(im1) != gdImageSX(im2) || gdImageSY(im1) != gdImageSY(im2)) {
		printf("Input has different size, please input images have same size\n");
		exit(-1);
	}
	width = gdImageSX(im1);
	height = gdImageSY(im2);
	im_new = gdImageCreateTrueColor(width,height);
	for(i=0;i<n;i++) {
		alpha = i * (1.0 / (double)(n-1));
		for(y=0;y<height;y++) {
			for(x=0;x<width;x++) {
				pixel1 = gdImageGetPixel(im1,x,y);
				pixel2 = gdImageGetPixel(im2,x,y);
				r = (int)(alpha * (double)gdImageRed(im1,pixel1) + (1.0 - alpha) * (double)gdImageRed(im2,pixel2));
				g = (int)(alpha * (double)gdImageGreen(im1,pixel1) + (1.0 - alpha) * (double)gdImageGreen(im2,pixel2));
				b = (int)(alpha * (double)gdImageBlue(im1,pixel1) + (1.0 - alpha) * (double)gdImageBlue(im2,pixel2));
				color = gdImageColorExact(im_new,r,g,b);
				gdImageSetPixel(im_new,x,y,color);
			}
		}
		sprintf(filename,"%s%02d.JPG",prefix,i);
		out = fopen(filename,"wb");
		gdImageJpeg(im_new,out,-1);
		fclose(out);
	}

	fclose(in1);
	fclose(in2);

	return 0;
}
