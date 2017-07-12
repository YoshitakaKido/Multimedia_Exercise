#include "image.h"

/* long int hist[256]; */
void make_histogram(int n);
void make_histogram_image(int h, int n);


main(int ac,char *av[]){

    char *fname = "rgb.bmp";
    char *wname = "out.bmp";
    int x,y;
    ImageData *img; //入力画像
    ImageData *outimg;
    Pixel *iro;
    iro=0;


    //ファイルの読み込み
    readBMPfile(fname, &img);
    outimg = createImage(img->width, img->height, img->depth);
    printf("read[%s]\n", fname);

    for(y=0; y < img->height ;y++){
      for(x=0;x < img->width ;x++){
        printf("a\n");
        getPixel(img,x,y,iro);
        printf("a\n");
        setPixel(outimg,x,y,iro);
      }
    }
    //----------------ここから-----------------------
/* for(i=0; i<256; i++){ */
/*   hist[i] = 0; */
/* } */

/* for(i=0; i<img->width*img->height; i++){ */
/*   hist[img->data[i].r]++; */
/* } */
/* for(i=0; i<256; i++){ */
/*   for(j=0; j<(int)(256*hist[i]/(double)max); j++){ */
/*     pix->data[(256-j)*256 + i].r = */
/*     pix->data[(256-j)*256 + i].g = */
/*     pix->data[(256-j)*256 + i].b = 255; */
/*   } */
/* } */

    //----------------ここまで-----------------------
    //ファイル書き込み
    writeBMPfile(wname, outimg);
    printf("write[%s]\n", wname);

    //メモリ解放
    disposeImage(img);
    disposeImage(outimg);
}


/* void make_histogram(int n){ */

/*   int i,x,y; */

/*   for(i=0;i<height[n];i++) hist[i] = 0; */

/*   for(y=0;y<height[n];y++) */
/*     for(x=0;x<width[n];x++) */
/*       hist[image[n][x][y]]++; */
/* } */

/* void make_histogram_image(int h, int n) */
/* { */
/*   int i,x,y; */
/*   long int max; */
/*   int takasa; */

/*   width[n]=256; height[n]=h; */

/*   max=0; */
/*   for(i=0;i<256;i++) */
/*     if(hist[i]>max)max=hist[i]; */

/*   for(x=0;x<width[n];x++){ */
/*     takasa = (int)(h/(double)max*hist[x]); */
/*     if(takasa>h)takasa = h; */
/*     for(y=0;y<h;y++) */
/*       if(y<takasa)image[n][x][h-1-y]=0; */
/*       else image[n][x][h-1-y]=255; */
/*   } */
/* } */
