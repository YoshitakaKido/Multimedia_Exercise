#include"image.h"

void linear_trans(ImageData **img, ImageData **outimg);

main(int ac,char *av[]){

    char *fname = "rgb.bmp";
    char *wname = "out.bmp";
    
    ImageData *img; //入力画像
    ImageData *outimg;
    
    //ファイルの読み込み
    readBMPfile(fname, &img);
    
    outimg = createImage(img->width, img->height, img->depth);

    printf("read[%s]\n", fname);
    //----------------ここから-----------------------
    
    linear_trans(&img, &outimg);


    //----------------ここまで-----------------------
    //ファイル書き込み
    writeBMPfile(wname, outimg); 
    printf("write[%s]\n", wname);

    //メモリ解放
    disposeImage(img);
}


void linear_trans(ImageData **img, ImageData **outimg){
    int x, y;
    int max = 0, min = 255;
    Pixel *pix;
    // 最小階調値と最大階調値を求める
    for(y = 0; y <= outimg->height; y++){
        for(x = 0; x <= outimg->width; x++){
            getPixel(img, x, y, pix);
            if() 
        }
    }

}
