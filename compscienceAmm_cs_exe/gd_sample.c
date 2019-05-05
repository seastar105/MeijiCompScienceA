#include <gd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc,const char *argv[]){

  FILE *out,*in;
  gdImagePtr im,im_new;
  int width,height,i,j,color,r,g,b,pixel;

  if(argv[1]==NULL||argv[2]==NULL||!strcmp(argv[1],argv[2])){
    printf("argument error\n");
    exit(-1);
  }

  //�������ǻ��ꤵ�줿�ե�������ɤ߽Ф��Ѥ˥����ץ�
  if((in=fopen(argv[1],"r"))==NULL){
    printf("file open error for %s\n",argv[1]);
    exit(-1);
  }
  //��������ǻ��ꤵ�줿�ե������񤭽Ф��Ѥ˥����ץ�
  if((out=fopen(argv[2],"wb"))==NULL){
    printf("file open error for %s\n",argv[2]);
    exit(-1);
  }

  //im �˲������ɤ߹���
  im = gdImageCreateFromJpeg(in);

  //���ϲ����Υ����������
  width=gdImageSX(im);
  height=gdImageSY(im);

  //�������������Ѱ�
  im_new= gdImageCreateTrueColor(width,height);

  for(i=0;i<width;i++){
    for(j=0;j<height;j++){
      
      //im �� (i,j) �ˤ����륫�顼����ǥå����μ���
      pixel=gdImageGetPixel(im,i,j);
      
      //im �� (i,j) �ˤ����� r,g,b ���ͤ����
      r=gdImageRed(im,pixel);
      g=gdImageGreen(im,pixel);
      b=gdImageBlue(im,pixel);

      //r,g,b �ͤ��� color ��������
      color=gdImageColorExact(im_new,r,g,b);

      //im_new �� (i,j) �ˤ�����ԥ������ͤ� color ������
      gdImageSetPixel(im_new,i,j,color);
    }
  }
  
  gdImageJpeg(im_new,out,-1);

  fclose(in);
  fclose(out);

  return 0;
  
}
