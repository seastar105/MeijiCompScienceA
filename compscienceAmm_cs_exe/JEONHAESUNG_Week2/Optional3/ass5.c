#include<gd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MINIMUM -1000000000
#define MAXIMUM  1000000000

// since transformation is symmetric transformation, inverse transformation is same
double direct_transformation_x(int x, int y, double m) {
	return (1.0 - m * m)/(m * m + 1.0) * (double)x + 2.0 * m / (m * m + 1.0) * (double)y;
}

double direct_transformation_y(int x, int y, double m) {
	return (2.0 * m) / (m * m + 1.0) * (double)x + (m * m - 1) / (m * m + 1) * (double)y;
}

void transform(double vertices[][2], int width, int height, double m) {
	vertices[0][0] = direct_transformation_x(0,0,m); vertices[0][1] = direct_transformation_y(0,0,m);
	vertices[1][0] = direct_transformation_x(width-1,0,m); vertices[1][1] = direct_transformation_y(width-1,0,m);
	vertices[2][0] = direct_transformation_x(0,height-1,m); vertices[2][1] = direct_transformation_y(0,height-1,m);
	vertices[3][0] = direct_transformation_x(width-1,height-1,m); vertices[3][1] = direct_transformation_y(width-1,height-1,m);
}

int valid(int x, int y, int width, int height) {
	if( x<0 || x >= width) return 0;
	else if ( y<0 || y >= height) return 0;
	return 1;
}

int main(const int argc, const char *argv[]) {
    FILE *out, *in;
    gdImagePtr im,im_new;
    int width, height, i, j, pixel, color,r,g,b;
	int new_width, new_height;
	int x,y,xd,yd;
	int x_offset, y_offset;
	double slope_a, slope_b, m;
	double vertices[4][2];												// first column has x values, and second column has 
																		// y values. Both values are transformed result
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
	if(argc < 4) {
		printf("missing a or b, input correctly\n");
		exit(-1);
	}
	slope_a = atof(argv[3]);
	slope_b = atof(argv[4]);
	m = -(slope_a / slope_b);
	im = gdImageCreateFromJpeg(in);

	width = gdImageSX(im);
	height = gdImageSY(im);

	// need to figure out offset to use in parallel moving
	{
		transform(vertices,width,height,m);
		double x_max=MINIMUM, x_min=MAXIMUM;
		double y_max=MINIMUM, y_min=MAXIMUM;
		for(i = 0;i < 4;i++) {
			if(x_max < vertices[i][0]) x_max = vertices[i][0];
			if(x_min > vertices[i][0]) x_min = vertices[i][0];
			if(y_max < vertices[i][1]) y_max = vertices[i][1];
			if(y_min > vertices[i][1]) y_min = vertices[i][1];
		}
		new_width = (int)(x_max - x_min);
		new_height = (int)(y_max - y_min);
		x_offset = x_min;
		y_offset = y_min;
	}
	im_new = gdImageCreateTrueColor(new_width,new_height);
	for(yd=0;yd<new_height;yd++) {
		for(xd=0;xd<new_width;xd++) {
			x = (int)floor(direct_transformation_x(xd + x_offset, yd + y_offset, m) + 0.5);
			y = (int)floor(direct_transformation_y(xd + x_offset, yd + y_offset, m) + 0.5);
			if(!valid(x,y,width,height)) continue;
			pixel = gdImageGetPixel(im,x,y);
			r = gdImageRed(im,pixel);
			g = gdImageGreen(im,pixel);
			b = gdImageBlue(im,pixel);
			color = gdImageColorExact(im_new,r,g,b);
			gdImageSetPixel(im_new,xd,yd,color);
		}
	}

	gdImageJpeg(im_new,out,-1);

	fclose(in);
	fclose(out);

	return 0;
}
