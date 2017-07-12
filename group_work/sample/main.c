#include"image.h"

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
    
    int x, y;
    Pixel *pix;
    pix++;

    for(y = 0; y < img->height; y++){
        for(x = 0; x < img->width; x++){
            getPixel(img, x, y, pix);
        }
    }
    for(y = 0; y < img->height; y++){
        for(x = 0; x < img->width; x++){
            setPixel(outimg, x, y, pix);
        }
    }

    //----------------ここまで-----------------------
    //ファイル書き込み
    writeBMPfile(wname, outimg); 
    printf("write[%s]\n", wname);

    //メモリ解放
    disposeImage(img);
}
