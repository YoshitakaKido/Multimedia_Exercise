#include "image.h"


#define COLORMAX 256

void make_histogram(ImageData *img,Pixel *pix);
void make_histogram_image_full(ImageData *outimg, Pixel *pix);
void make_histgram_image_rgb(ImageData *outimg, Pixel *pix,int num);
void Initialization_histgram();
void save(ImageData *img,Pixel *pix,ImageData *outimg,int width[], int height[],int num);
void linear(ImageData *img,ImageData *outimg);

int hist[256][3]={{0},{0}};

main(int ac,char *av[]){

  char *fname = "rgb.bmp";//元の画像
  char *fname_linear = "out.bmp";//線形変換の画像
  char *wname_linear = "out.bmp";//線形変換後の画像
  char *wname_image = "out_image.bmp";//出力する画像
  char *wname_rgb = "rgb_hist.bmp";//赤、緑、青のそれぞれの色のヒストグラムの画像
  char *wname_color = "color_hist.bmp";//三原色を組み合わせたヒストグラムの画像
  ImageData *img; //画像
  ImageData *outrgb;
  ImageData *outrgb2;
  ImageData *outimg;
  ImageData *outcolor;
  ImageData *outlinear;
  Pixel *pix;

  //元の画像の読み込み
  readBMPfile(fname, &img);
  //座標の初期化
  int width[2]={0,img->width},height[2]={0,img->height};

  //Pixelの初期化
  pix=malloc(sizeof(void*));

  //画像データの比較画像
  outimg = createImage(img->width*2,img->height, img->depth);
  //線形変換後のデータ
  outlinear = createImage(img->width,img->height, img->depth);
  //ヒストグラムの画像データ
  outrgb=createImage(COLORMAX,COLORMAX,img->depth);
  //色別のヒストグラムの画像データ
  outrgb2=createImage(COLORMAX*2,COLORMAX*3,img->depth);
  //三原色のヒストグラムの画像データ
  outcolor = createImage(COLORMAX*2,COLORMAX,img->depth);

  //線形変換後の画像を作成
  linear(img,outlinear);
  //ファイルに書き込み
  writeBMPfile(wname_linear, outlinear);




  //画像データの保存
  save(img,pix,outimg,width,height,1);

  //hist
  make_histogram(img,pix);
  make_histogram_image_full(outrgb,pix);
  //保存する座標を決定
  width[1]=COLORMAX;
  height[1]=COLORMAX;
  //画像データの保存
  save(outrgb,pix,outcolor,width,height,1);

  //Red_histgram
  make_histgram_image_rgb(outrgb,pix,0);
  //保存する座標を決定
  width[0]=0;
  width[1]=COLORMAX;
  height[1]=COLORMAX;
  //画像データの保存
  save(outrgb,pix,outrgb2,width,height,1);

  //Green_histgram
  make_histgram_image_rgb(outrgb,pix,1);
  //保存する座標を決定
  height[0]=COLORMAX;
  height[1]=COLORMAX*2;
  //画像データの保存
  save(outrgb,pix,outrgb2,width,height,1);

  //Blue_histgram
  make_histgram_image_rgb(outrgb,pix,2);
  //保存する座標を決定
  height[0]=COLORMAX*2;
  height[1]=COLORMAX*3;
  //画像データの保存
  save(outrgb,pix,outrgb2,width,height,1);


  //ここから線形変換-----------------------------------------------

  //ヒストグラムの初期化
  Initialization_histgram();


  //線形変換後の画像のファイル読み込み
  readBMPfile(fname_linear,&img);


  //保存する座標を決定
  width[0]=img->width;
  width[1]=img->width*2;
  height[0]=0;
  height[1]=img->height;
  //画像データの保存
  save(outlinear,pix,outimg,width,height,1);

  //hist
  make_histogram(outlinear,pix);
  make_histogram_image_full(outrgb,pix);
  //保存する座標を決定
  width[0]=COLORMAX;
  width[1]=COLORMAX*2;
  height[0]=0;
  height[1]=COLORMAX;
  //画像データを保存
  save(outrgb,pix,outcolor,width,height,1);

  //Red_histgram
  make_histgram_image_rgb(outrgb,pix,0);
  //保存する座標を決定
  width[0]=COLORMAX;
  width[1]=COLORMAX*2;
  height[1]=COLORMAX;
  //画像データを保存
  save(outrgb,pix,outrgb2,width,height,1);

  //Green_histgram
  make_histgram_image_rgb(outrgb,pix,1);
  //保存する座標を決定
  height[0]=COLORMAX;
  height[1]=COLORMAX*2;
  //画像データを保存
  save(outrgb,pix,outrgb2,width,height,1);

  //Blue_histgram
  make_histgram_image_rgb(outrgb,pix,2);
  //保存する座標を決定
  height[0]=COLORMAX*2;
  height[1]=COLORMAX*3;
  //画像データを保存
  save(outrgb,pix,outrgb2,width,height,1);

  //ファイル書き込み
  writeBMPfile(wname_image, outimg);
  writeBMPfile(wname_rgb, outrgb2);
  writeBMPfile(wname_color, outcolor);

  //領域の解放
  free(pix);
  disposeImage(img);
  disposeImage(outimg);
  disposeImage(outrgb);
  disposeImage(outcolor);
  disposeImage(outlinear);

}


/* ヒストグラムの作成 */
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

/* 三原色のヒストグラムの画像を作成 */
void make_histogram_image_full(ImageData *outimg,Pixel *pix){
  Pixel *max;
  Pixel *takasa;
  int max_color;
  int x,y,i;
  //領域の確保
  max=malloc(sizeof(void*));
  takasa=malloc(sizeof(void*));
  //初期化
  max->r=PIXELMIN;
  max->g=PIXELMIN;
  max->b=PIXELMIN;
  //三原色それぞれの最大値の決定
  for(i=0;i<COLORMAX;i++){
    if(hist[i][0]>max->r)max->r=hist[i][0];
    if(hist[i][1]>max->g)max->g=hist[i][1];
    if(hist[i][2]>max->b)max->b=hist[i][2];
  }
  //色の階調値の最大数を決定
  max_color=max->r;
  if(max_color<max->g)max_color=max->g;
  if(max_color<max->b)max_color=max->b;

  //ヒストグラムの作成
  for(y=0;y<COLORMAX;y++){
    for(x=0;x<COLORMAX;x++){
      takasa->r = (int)(COLORMAX/(double)max_color*hist[x][0]);
      takasa->g = (int)(COLORMAX/(double)max_color*hist[x][1]);
      takasa->b = (int)(COLORMAX/(double)max_color*hist[x][2]);
      if(takasa->r>=COLORMAX)takasa->r=COLORMAX;
      if(takasa->g>=COLORMAX)takasa->g=COLORMAX;
      if(takasa->b>=COLORMAX)takasa->b=COLORMAX;


      if(y<takasa->r)pix->r=PIXELMAX;
      else pix->r = PIXELMIN;
      if(y<takasa->g)pix->g=PIXELMAX;
      else pix->g = PIXELMIN;
      if(y<takasa->b)pix->b=PIXELMAX;
      else pix->b = PIXELMIN;
      setPixel(outimg,x,COLORMAX-1-y,pix);
    }
  }
  //領域の解放
  free(max);
  free(takasa);
}

/* 色別のヒストグラムの画像を作成 */
void make_histgram_image_rgb(ImageData *outimg, Pixel *pix,int num){
  int takasa;
  int max_color;
  int x,y,i;
  //初期化
  max_color=PIXELMIN;
  pix->r=PIXELMIN;
  pix->g=PIXELMIN;
  pix->b=PIXELMIN;
  //色の最大値を決定
  for(i=0;i<COLORMAX;i++){
    if(hist[i][num]>max_color)max_color=hist[i][num];
  }
  //選んだ色のヒストグラムを作成
  for(y=0;y<COLORMAX;y++){
    for(x=0;x<COLORMAX;x++){
      takasa = (int)(COLORMAX/(double)max_color*hist[x][num]);
      if(takasa>=COLORMAX)takasa=COLORMAX;

      //選んだ色の最大のヒストグラムの高さを決定
      switch(num){
      case 0:
        if(y<takasa)pix->r=PIXELMAX;
        else pix->r = PIXELMIN;
        break;
      case 1:
        if(y<takasa)pix->g=PIXELMAX;
        else pix->g = PIXELMIN;
        break;
      case 2:
        if(y<takasa)pix->b=PIXELMAX;
        else pix->b = PIXELMIN;
        break;
      default:
        printf("赤:0 緑:1 青:2にしてください。\n");
        break;
      }
      //画像データの保存
      setPixel(outimg,x,COLORMAX-1-y,pix);
    }
  }
}



//ヒストグラムの初期化
void Initialization_histgram(){
  int i,col;
  for(i=0;i<COLORMAX;i++)
    for(col=0;col<3;col++)
      hist[i][col]=0;
}

/* imgの画像データをoutimgに保存させる
 width[0]:outimgのx座標の始点
 width[1]:outimgのx座標の終点
 height[0]:outimgのy座標の始点
 height[1]:outimgのy座標の終点
*/
void save(ImageData *img,Pixel *pix,ImageData *outimg,int width[], int height[],int num){
  int x,y;

  for(y=height[0];y<height[1];y++){
    for(x=width[0];x<width[1];x++){

      getPixel(img, (x/num%img->width), (y/num%img->height), pix);
      setPixel(outimg,x,y,pix);
    }
  }
}


/* img:線形変換前の画像
   outimg:線形変換後の画像*/
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

    //画像データの保存
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
