#include "image.h"

void linear(ImageData *img, ImageData *outimg);

main(int ac,char *av[]){

    char *fname = "rgb.bmp";
    char *wname = "out.bmp";

    ImageData *img;
    ImageData *outimg;

    //ファイルの読み込み
    readBMPfile(fname, &img);

    outimg = createImage(img->width, img->height, img->depth);

    printf("read[%s]\n", fname);

    //----------------ここから-----------------------

    linear(img, outimg);

    //----------------ここまで-----------------------

    //ファイル書き込み
    writeBMPfile(wname, outimg);
    printf("write[%s]\n", wname);

    //メモリ解放
    disposeImage(img);
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
