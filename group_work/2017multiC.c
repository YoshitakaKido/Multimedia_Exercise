#include "image.h"


#define COLORMAX 256

void make_histogram(ImageData *img,Pixel *pix);
void make_histogram_image_full(ImageData *outimg, Pixel *pix);
void make_histogram_image_r(ImageData *outimg, Pixel *pix);
void make_histogram_image_g(ImageData *outimg, Pixel *pix);
void make_histogram_image_b(ImageData *outimg, Pixel *pix);
void Initialization_histgram();
void save(ImageData *img,Pixel *pix,ImageData *outimg,int width[], int height[]);


void linear(ImageData *img,ImageData *outimg);

int hist[256][3]={{0},{0}};


main(int ac,char *av[]){

  char *fname = "rgb.bmp";//元の画像
  char *fname_linear = "out.bmp";//線形変換の画像
  char *wname_linear = "out.bmp";//線形変換後の画像
  char *wname_image = "out_image.bmp";//出力する画像
  char *wname_rgb = "rgb_hist.bmp";//赤、緑、青のそれぞれの色のヒストグラムの画像
  char *wname_color = "color_hist.bmp";//三原色を組み合わせたヒストグラムの画像
  ImageData *img; //入力画像
  ImageData *outrgb;
  ImageData *outrgb2;
  ImageData *outimg;
  ImageData *outcolor;
  ImageData *outlinear;
  Pixel *pix;

  pix=malloc(sizeof(void*));

  //元の画像の読み込み
  readBMPfile(fname, &img);
  int width[2]={0,img->width},height[2]={0,img->height};
  outimg = createImage(img->width*2,img->height, img->depth);
  outlinear = createImage(img->width,img->height, img->depth);
  outrgb=createImage(COLORMAX,COLORMAX,img->depth);
  outrgb2=createImage(COLORMAX*2,COLORMAX*3,img->depth);
  outcolor = createImage(COLORMAX*2,COLORMAX,img->depth);
  //線形変換後の画像を出力
  linear(img,outlinear);
  writeBMPfile(wname_linear, outlinear);

  //img
  printf("read[%s]\n", fname);
  save(img,pix,outimg,width,height);

  //hist
  make_histogram(img,pix);
  make_histogram_image_full(outrgb,pix);
  width[1]=COLORMAX;
  height[1]=COLORMAX;
  save(outrgb,pix,outcolor,width,height);
  //Red_histgram
  make_histogram_image_r(outrgb,pix);
  width[0]=0;
  width[1]=COLORMAX;
  height[1]=COLORMAX;
  save(outrgb,pix,outrgb2,width,height);
  //Green_histgram
  make_histogram_image_g(outrgb,pix);
  height[0]=COLORMAX;
  height[1]=COLORMAX*2;
  save(outrgb,pix,outrgb2,width,height);
  //Blue_histgram
  make_histogram_image_b(outrgb,pix);
  height[0]=COLORMAX*2;
  height[1]=COLORMAX*3;
  save(outrgb,pix,outrgb2,width,height);



  Initialization_histgram();
  //線形変換

  readBMPfile(fname_linear,&img);


  printf("read[%s]\n", fname_linear);
  width[0]=img->width;
  width[1]=img->width*2;
  height[0]=0;
  height[1]=img->height;
  save(outlinear,pix,outimg,width,height);

  //hist
  make_histogram(outlinear,pix);
  make_histogram_image_full(outrgb,pix);
  width[0]=COLORMAX;
  width[1]=COLORMAX*2;
  height[0]=0;
  height[1]=COLORMAX;
  save(outrgb,pix,outcolor,width,height);
  //Red_histgram
  make_histogram_image_r(outrgb,pix);
  width[0]=COLORMAX;
  width[1]=COLORMAX*2;
  height[1]=COLORMAX;
  save(outrgb,pix,outrgb2,width,height);
  //Green_histgram
  make_histogram_image_g(outrgb,pix);
  height[0]=COLORMAX;
  height[1]=COLORMAX*2;
  save(outrgb,pix,outrgb2,width,height);
  //Blue_histgram
  make_histogram_image_b(outrgb,pix);
  height[0]=COLORMAX*2;
  height[1]=COLORMAX*3;
  save(outrgb,pix,outrgb2,width,height);
  writeBMPfile(wname_image, outimg);
  writeBMPfile(wname_rgb, outrgb2);
  writeBMPfile(wname_color, outcolor);

  //ファイル書き込み

  free(pix);

  //メモリ解放
  disposeImage(img);
  disposeImage(outimg);
  disposeImage(outrgb);
  disposeImage(outcolor);
  disposeImage(outlinear);
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

void save(ImageData *img,Pixel *pix,ImageData *outimg,int width[], int height[]){
  int x,y;

  for(y=height[0];y<height[1];y++){
    for(x=width[0];x<width[1];x++){

      getPixel(img, (x%img->width), (y%img->height), pix);

      setPixel(outimg,x,y,pix);
    }
  }
}



void linear(ImageData *img,ImageData *outimg) {
    int x,y; //for文用
    int max[3] = {0}; //全要素0で初期化
    int min[3] = {255,255,255};
    Pixel pix; //r,g,b

    //最大・最小階調を調べる
    for(x=0;x<img->width;x++) {
        for(y=0;y<img->height;y++) {
            getPixel(img,x,y,&pix);
            if(max[0] < pix.r) {
                max[0] = pix.r;
            }
            if(max[1] < pix.g) {
                max[1] = pix.g;
            }
            if(max[2] < pix.b) {
                max[2] = pix.b;
            }
            if(min[0] > pix.r) {
                min[0] = pix.r;
            }
            if(min[1] > pix.g) {
                min[1] = pix.g;
            }
            if(min[2] > pix.b) {
                min[2] = pix.b;
            }
        }
    }

    for(x=0;x<img->width;x++) {
        for(y=0;y<img->height;y++) {
            getPixel(img,x,y,&pix);
            pix.r = (int)(((double)(pix.r - min[0]) / (max[0] - min[0])) * 255);
            pix.g = (int)(((double)(pix.g - min[1]) / (max[1] - min[1])) * 255);
            pix.b = (int)(((double)(pix.b - min[2]) / (max[2] - min[2])) * 255);
            setPixel(outimg,x,y,&pix);
        }
    }
}
