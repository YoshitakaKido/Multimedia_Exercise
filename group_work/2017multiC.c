#include "image.h"


#define COLORMAX 256

void make_histogram(ImageData *img,Pixel *pix);
void make_histogram_image_full(ImageData *outimg, Pixel *pix);
void make_histogram_image_r(ImageData *outimg, Pixel *pix);
void make_histogram_image_g(ImageData *outimg, Pixel *pix);
void make_histogram_image_b(ImageData *outimg, Pixel *pix);
void Initialization_histgram();
void save(ImageData *img,Pixel *pix,ImageData *outimg,int num,int width[], int height[]);
int hist[256][3]={{0},{0}};


main(int ac,char *av[]){

  char *fname = "rgb.bmp";
  char *fname_linear = "out.bmp";
  char *wname_image = "out_image.bmp";
  char *wname_rgb = "rgb_hist.bmp";
  ImageData *img; //入力画像
  ImageData *outrgb;
  ImageData *outrgb2;
  ImageData *outimg;
 Pixel *pix;

  pix=malloc(sizeof(void*));

  //ファイルの読み込み
  readBMPfile(fname, &img);
  int width[2]={0,img->width},height[2]={0,img->height};
  outimg = createImage(img->width*2,img->height, img->depth);
  outrgb=createImage(COLORMAX,COLORMAX,img->depth);
  outrgb2=createImage(COLORMAX*8,COLORMAX*3,img->depth);
//img
  printf("read[%s]\n", fname);
  save(img,pix,outimg,1,width,height);

  //hist
  make_histogram(img,pix);
  make_histogram_image_full(outrgb,pix);
  width[1]=COLORMAX*3;
  height[1]=COLORMAX*3;
  save(outrgb,pix,outrgb2,3,width,height);
  //Red_histgram
  make_histogram_image_r(outrgb,pix);
  width[0]=COLORMAX*6;
  width[1]=COLORMAX*7;
  height[1]=COLORMAX;
  save(outrgb,pix,outrgb2,1,width,height);
  //Green_histgram
  make_histogram_image_g(outrgb,pix);
  height[0]=COLORMAX;
  height[1]=COLORMAX*2;
  save(outrgb,pix,outrgb2,1,width,height);
  //Blue_histgram
  make_histogram_image_b(outrgb,pix);
  height[0]=COLORMAX*2;
  height[1]=COLORMAX*3;
  save(outrgb,pix,outrgb2,1,width,height);


  Initialization_histgram();
  readBMPfile(fname_linear, &img);



  printf("read[%s]\n", fname);
  width[0]=img->width;
  width[1]=img->width*2;
  height[0]=0;
  height[1]=img->height;
  save(img,pix,outimg,1,width,height);

  //hist
  make_histogram(img,pix);
  make_histogram_image_full(outrgb,pix);
  width[0]=COLORMAX*3;
  width[1]=COLORMAX*6;
  height[0]=0;
  height[1]=COLORMAX*3;
  save(outrgb,pix,outrgb2,3,width,height);
  //Red_histgram
  make_histogram_image_r(outrgb,pix);
  width[0]=COLORMAX*7;
  width[1]=COLORMAX*8;
  height[1]=COLORMAX;
  save(outrgb,pix,outrgb2,1,width,height);
  //Green_histgram
  make_histogram_image_g(outrgb,pix);
  height[0]=COLORMAX;
  height[1]=COLORMAX*2;
  save(outrgb,pix,outrgb2,1,width,height);
  //Blue_histgram
  make_histogram_image_b(outrgb,pix);
  height[0]=COLORMAX*2;
  height[1]=COLORMAX*3;
  save(outrgb,pix,outrgb2,1,width,height);
  writeBMPfile(wname_image, outimg);
  writeBMPfile(wname_rgb, outrgb2);

  //ファイル書き込み
  /* writeBMPfile(wname_image, outimg); */
  /* printf("write[%s]\n", wname_image); */
  /* writeBMPfile(wname_rgb, outrgb); */
  /* Initialization_histgram(); */

  /* readBMPfile(fname_linear, &img); */
  /* make_histogram(img,pix); */
  /* make_histogram_image_full(outimg,pix); */
  /* writeBMPfile(wname_linear, outimg); */
  /* printf("write[%s]\n", wname_f); */

  /* make_histogram_image_r(outimg,pix); */
  /* writeBMPfile(wname_r, outimg); */
  /* printf("write[%s]\n", wname_r); */

  /* make_histogram_image_g(outimg,pix); */
  /* writeBMPfile(wname_g, outimg); */
  /* printf("write[%s]\n", wname_g); */

  /* make_histogram_image_b(outimg,pix); */
  /* writeBMPfile(wname_b, outimg); */
  /* printf("write[%s]\n", wname_b); */




  free(pix);

  //メモリ解放
  disposeImage(img);
  disposeImage(outimg);
  disposeImage(outrgb);
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

void Initialization_histgram(){
  int i,col;
  for(i=0;i<COLORMAX;i++)
    for(col=0;col<3;col++)
      hist[i][col]=0;
}

void save(ImageData *img,Pixel *pix,ImageData *outimg,int num,int width[], int height[]){
  int x,y;
  for(y=height[0];y<height[1];y++){
    for(x=width[0];x<width[1];x++){
      getPixel(img, (x/num%img->width), (y/num%img->height), pix);
      setPixel(outimg,x,y,pix);
    }
  }
}
