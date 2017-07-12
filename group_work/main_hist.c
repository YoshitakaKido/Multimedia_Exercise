#include "image.h"


#define COLORMAX 256

void make_histogram(ImageData *img,Pixel *pix);
void make_histogram_image_full(ImageData *outimg, Pixel *pix);
void make_histogram_image_r(ImageData *outimg, Pixel *pix);
void make_histogram_image_g(ImageData *outimg, Pixel *pix);
void make_histogram_image_b(ImageData *outimg, Pixel *pix);
int hist[256][3]={{0},{0}};


main(int ac,char *av[]){

  char *fname = "rgb.bmp";
  char *fname_linear = "out.bmp";
  char *wname_f = "out_hist_full.bmp";
  char *wname_linear = "out_hist_linear.bmp";
  char *wname_r = "out_hist_r.bmp";
  char *wname_g = "out_hist_g.bmp";
  char *wname_b = "out_hist_b.bmp";
  ImageData *img; //入力画像
  ImageData *outimg;
  Pixel *pix;

  pix=malloc(sizeof(void*));

  //ファイルの読み込み
  readBMPfile(fname, &img);
  outimg = createImage(COLORMAX, COLORMAX, img->depth);
  printf("read[%s]\n", fname);

  make_histogram(img,pix);
  make_histogram_image_full(outimg,pix);

  //ファイル書き込み
  writeBMPfile(wname_f, outimg);
  printf("write[%s]\n", wname_f);

  readBMPfile(fname_linear, &img);
  make_histogram(img,pix);
  make_histogram_image_full(outimg,pix);
  writeBMPfile(wname_linear, outimg);
  printf("write[%s]\n", wname_f);

  make_histogram_image_r(outimg,pix);
  writeBMPfile(wname_r, outimg);
  printf("write[%s]\n", wname_r);

  make_histogram_image_g(outimg,pix);
  writeBMPfile(wname_g, outimg);
  printf("write[%s]\n", wname_g);

  make_histogram_image_b(outimg,pix);
  writeBMPfile(wname_b, outimg);
  printf("write[%s]\n", wname_b);




  free(pix);

  //メモリ解放
  disposeImage(img);
  disposeImage(outimg);
}
void make_histogram(ImageData *img,Pixel *pix){
  int x,y;
  for(y=0; y < img->height ;y++){
    for(x=0;x < img->width ;x++){
      getPixel(img,x,y,pix);
      hist[pix->r][0]++;
      hist[pix->g][1]++;
      hist[pix->b][2]++;
    }
  }
}
void make_histogram_image_full(ImageData *outimg,Pixel *pix){
  Pixel *max;
  Pixel *takasa;
  int max_color;
  int x,y,i;
  max=malloc(sizeof(void*));
  takasa=malloc(sizeof(void*));
  max->r=PIXELMIN;
  max->g=PIXELMIN;
  max->b=PIXELMIN;
  for(i=0;i<COLORMAX;i++){
    if(hist[i][0]>max->r)max->r=hist[i][0];
    if(hist[i][1]>max->g)max->g=hist[i][1];
    if(hist[i][2]>max->b)max->b=hist[i][2];
  }
  max_color=max->r;
  if(max_color<max->g)max_color=max->g;
  if(max_color<max->b)max_color=max->b;
  for(y=0;y<COLORMAX;y++){
    for(x=0;x<COLORMAX;x++){
      takasa->r = (int)(COLORMAX/(double)max_color*hist[x][0]);
      takasa->g = (int)(COLORMAX/(double)max_color*hist[x][1]);
      takasa->b = (int)(COLORMAX/(double)max_color*hist[x][2]);
      if(takasa->r>COLORMAX)takasa->r=COLORMAX;
      if(takasa->g>COLORMAX)takasa->g=COLORMAX;
      if(takasa->b>COLORMAX)takasa->b=COLORMAX;


      if(y<takasa->r)pix->r=PIXELMAX;
      else pix->r = PIXELMIN;
      if(y<takasa->g)pix->g=PIXELMAX;
      else pix->g = PIXELMIN;
      if(y<takasa->b)pix->b=PIXELMAX;
      else pix->b = PIXELMIN;
      setPixel(outimg,x,COLORMAX-1-y,pix);

    }
  }

  free(max);
  free(takasa);

}
void make_histogram_image_r(ImageData *outimg, Pixel *pix){
  Pixel *max;
  Pixel *takasa;
  int max_color;
  int x,y,i;
  max=malloc(sizeof(void*));
  takasa=malloc(sizeof(void*));
  max->r=PIXELMIN;
  pix->g=PIXELMIN;
  pix->b=PIXELMIN;
  for(i=0;i<COLORMAX;i++){
    if(hist[i][0]>max->r)max->r=hist[i][0];
  }
  max_color=max->r;
  for(y=0;y<COLORMAX;y++){
    for(x=0;x<COLORMAX;x++){
      takasa->r = (int)(COLORMAX/(double)max_color*hist[x][0]);
      if(takasa->r>COLORMAX)takasa->r=COLORMAX;


      if(y<takasa->r)pix->r=PIXELMAX;
      else pix->r = PIXELMIN;

      setPixel(outimg,x,COLORMAX-1-y,pix);
    }
  }

  free(max);
  free(takasa);

}
void make_histogram_image_g(ImageData *outimg, Pixel *pix){
  Pixel *max;
  Pixel *takasa;
  int max_color;
  int x,y,i;
  max=malloc(sizeof(void*));
  takasa=malloc(sizeof(void*));
  max->g=PIXELMIN;
  pix->r=PIXELMIN;
  pix->b=PIXELMIN;
  for(i=0;i<COLORMAX;i++){
    if(hist[i][1]>max->g)max->g=hist[i][1];
  }
  max_color=max->g;
  for(y=0;y<COLORMAX;y++){
    for(x=0;x<COLORMAX;x++){
      takasa->g = (int)(COLORMAX/(double)max_color*hist[x][1]);
      if(takasa->g>COLORMAX)takasa->g=COLORMAX;

      if(y<takasa->g)pix->g=PIXELMAX;
      else pix->g = PIXELMIN;

      setPixel(outimg,x,COLORMAX-1-y,pix);
    }
  }

  free(max);
  free(takasa);
}
void make_histogram_image_b(ImageData *outimg, Pixel *pix){
  Pixel *max;
  Pixel *takasa;
  int max_color;
  int x,y,i;
  max=malloc(sizeof(void*));
  takasa=malloc(sizeof(void*));
  max->b=PIXELMIN;
  pix->r=PIXELMIN;
  pix->g=PIXELMIN;
  for(i=0;i<COLORMAX;i++){
    if(hist[i][2]>max->b)max->b=hist[i][2];
  }
  max_color=max->b;
  for(y=0;y<COLORMAX;y++){
    for(x=0;x<COLORMAX;x++){
      takasa->b = (int)(COLORMAX/(double)max_color*hist[x][2]);
      if(takasa->b>COLORMAX)takasa->b=COLORMAX;

      if(y<takasa->b)pix->b=PIXELMAX;
      else pix->b = PIXELMIN;

      setPixel(outimg,x,COLORMAX-1-y,pix);
    }
  }

  free(max);
  free(takasa);
}
