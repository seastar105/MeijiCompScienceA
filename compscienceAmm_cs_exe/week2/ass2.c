#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    FILE *out, *in, *fin;
    gdImagePtr im,im_pad,im_new;
    int width, height, i, j, k, l, pixel, color,r,g,b;
	double sum_r, sum_g, sum_b;
	int w,col=0,row=0;
	double **filter;
	char buf[BUF_MAX];
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
	if((fin=fopen(argv[3],"r")) == NULL) {
		printf("file open error for %s\n",argv[3]);
		exit(-1);
	}
	im = gdImageCreateFromJpeg(in);

	width = gdImageSX(im);
	height = gdImageSY(im);

	// need to read filter here
	fgets(buf,BUF_MAX,fin); row++;
	char *ptr = strtok(buf,",");
	while(ptr != NULL) {
		col++;
		ptr = strtok(NULL,",");
	}
	while(fgets(buf,BUF_MAX,fin)) row++;
	if(row != col) {
		printf("Please Input right filter, row and column has different number\n");
		exit(-1);
	}

	filter=(double**)malloc(sizeof(double*)*row);
	for(i=0;i<row;i++) filter[i] = (double*)malloc(sizeof(double) * col);

	rewind(fin);
	i=0;
	while(fgets(buf,BUF_MAX,fin)) {
		ptr = strtok(buf,",");
		j = 0;
		while(ptr != NULL) {
			sscanf(ptr,"%lf,",&filter[i][j++]);
			ptr = strtok(NULL,",");
		}
		i++;
	}
	w = row/2;
	im_pad = gdImageCreateTrueColor(width+2*w,height+2*w);
	gdImageCopy(im_pad,im,w,w,0,0,width,height);
	im_new = gdImageCreateTrueColor(width,height);
	makeGrayScale(im_pad);
	for(j=w;j<height+w;j++) {
		for(i=w;i<width+w;i++) {
			sum_r = sum_g = sum_b = 0.0;
			for(l=0;l<=2*w;l++) {
				for(k=0;k<=2*w;k++) {
					pixel = gdImageGetPixel(im_pad,i-w+k,j-w+l);
					sum_r += (double)gdImageRed(im_pad,pixel) * filter[l][k];
					sum_g += (double)gdImageGreen(im_pad,pixel) * filter[l][k];
					sum_b += (double)gdImageBlue(im_pad,pixel) * filter[l][k];
				}
			}
			color = gdImageColorExact(im_new,(int)sum_r,(int)sum_g,(int)sum_b);
			gdImageSetPixel(im_new,i-w,j-w,color);
		}
	}
	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);
	fclose(fin);
	return 0;
}
