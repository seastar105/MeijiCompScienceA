#include <gd.h>
#include <stdio.h>

int main(void){

  FILE *jpegout,*pngout;
  int black;
  int white;
  gdImagePtr im;

  im = gdImageCreate(64,64);

  black=gdImageColorAllocate(im,0,0,0);
  white=gdImageColorAllocate(im,255,255,255);

  gdImageLine(im,0,0,63,63,white);

  printf("%d\n2",gdImageGetPixel(im,-5,-5));

  pngout=fopen("test.png","wb");
  jpegout=fopen("test.jpg","wb");

  gdImagePng(im,pngout);
  gdImageJpeg(im,jpegout,-1);

  return 0;
  
}
