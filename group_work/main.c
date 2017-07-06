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
   


    //----------------ここまで-----------------------
    //ファイル書き込み
    writeBMPfile(wname, outimg); 
    printf("write[%s]\n", wname);

    //メモリ解放
    disposeImage(img);
}
