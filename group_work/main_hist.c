#include "image.h"


#define COLORMAX 256

void make_histogram(int n);
void make_histogram_image(int h, int n);


main(int ac,char *av[]){

    char *fname = "rgb.bmp";
    char *wname = "out_hist.bmp";
    int x,y,i;
    ImageData *img; //入力画像
    ImageData *outimg;
    Pixel *pix;
    int hist[256][3]={{0},{0}};
    Pixel *max;
    Pixel *takasa;
    int max_color;
    pix=malloc(sizeof(void*));
    max=malloc(sizeof(void*));
    takasa=malloc(sizeof(void*));

    //ファイルの読み込み
    readBMPfile(fname, &img);
    outimg = createImage(COLORMAX, COLORMAX, img->depth);
    printf("read[%s]\n", fname);

    for(y=0; y < img->height ;y++){
      for(x=0;x < img->width ;x++){
        getPixel(img,x,y,pix);
        hist[pix->r][0]++;
        hist[pix->g][1]++;
        hist[pix->b][2]++;
        //        setPixel(outimg,x,y,pix);
      }
    }
    max->r=PIXELMIN;
    max->g=PIXELMIN;
    max->b=PIXELMIN;



    //----------------ここから-----------------------
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

        /* max_takasa=takasa->r; */
        /* if(max_takasa<takasa->g)max_takasa=takasa->g; */
        /* if(max_takasa<takasa->b)max_takasa=takasa->b; */
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
    //----------------ここまで-----------------------
    //ファイル書き込み
    writeBMPfile(wname, outimg);
    printf("write[%s]\n", wname);
    free(pix);
    free(max);
    free(takasa);

    //メモリ解放
    disposeImage(img);
    disposeImage(outimg);
}
