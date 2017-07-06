#include "image.h"
typedef unsigned long   DWORD;
typedef int             BOOL;
typedef unsigned short  WORD;
typedef unsigned long   LONG;


#define BI_RGB          0L
#define BI_RLE8         1L
#define BI_RLE4         2L
#define BI_BITFIELDS    3L

typedef struct tagBITMAPFILEHEADER{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bf0ffBits;

} BITMAPFILEHEADER, *PBITMAPFILEHEADER;


typedef struct tagBITMAPCOREHEADER{
  DWORD bcSize;
  WORD bcWidth;
  WORD bcHeight;
  WORD bcPlanes;
  WORD bcBitCount;
}BITMAPCOREHEADER,*PBITMAPCOREHEADER;

typedef struct tagBITMAPINFOHEADER{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#define MAXCOLORS 256

/* ファイルより2バイト整数を書き込む(リトルエンディアン) */
int fwriteWORD(WORD val, FILE *fp)
{
  int i, c;

  c = val;
  for(i=0;i<2;i++){
    fputc(c % 256, fp);
    c /= 256;
  }
  return TRUE;
}

/* ファイルより4バイト整数を書き込む(リトルエンディアン) */
int fwriteDWORD(DWORD val, FILE *fp)
{
  int i, c;

  c = val;
  for(i=0;i<4;i++){
    fputc(c % 256,fp);
    c /= 256;
  }
  return TRUE;
}

/* ファイルより2バイト整数を読み込む(リトルエンディアン) */
int freadWORD(WORD *res, FILE *fp)
{
  int i, c;
  int val[2];

  for(i=0;i<2;i++){
    c = fgetc(fp);
    if(c == EOF) return FALSE;
    val[i] = c;
  }
  *res = val[1]*256+val[0];
  return TRUE;
}

/* ファイルより4バイト整数を読み込む(リトルエンディアン) */
int freadDWORD(DWORD *res, FILE *fp)
{
  int i, c;
  int val[4];
  DWORD tmp = 0;


  for(i = 0; i < 4; i++){
    c = fgetc(fp);
    if(c == EOF) return FALSE;
    val[i] = c;
  }

  tmp = 0;
  for(i = 3; i >= 0; i--){
    tmp *= 256;
    tmp += val[i];
  }

  *res = tmp;
  return TRUE;
}

static BOOL IsWinDIB(BITMAPINFOHEADER* pBIH)
{
  if(((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER)){
    return FALSE;
  }
  return TRUE;
}

int count0fDIBColorEntries(int iBitCount)
{
  int iColors;

  switch(iBitCount){
  case 1:
    iColors=2;
    break;
  case 4:
    iColors = 16;
    break;
  case 8:
    iColors =256;
    break;
  default :
    iColors = 0;
    break;
  }

  return iColors;
}


int getDIBxmax(int mx, int dep)
{
  switch(dep){
  case 32:
    return mx*4;
  case 24:
    return ((mx*3)+3)/4*4;
  case 16:
    return (mx+1)/2*2;
    break;
  case 8:
    return (mx+3)/4*4;
  case 4:
    return (((mx+1)/2)+3)/4*4;
    break;
  case 1:
    return (((mx+7)/8)+3)/4*4;
  }
  return mx;
}
// BMPデータをファイルより読み込む
int readBMPfile(char *fname,ImageData **img)
{
  int i,c;
  int errcode = 0;
  BITMAPFILEHEADER BMPFile;
  int fsize;
  BITMAPINFOHEADER BMPInfo;
  BITMAPCOREHEADER BMPCore;
  int colors;
  int colorTableSize;
  int bitsSize;
  int BISize;
  int x, y;
  int mx, my, depth;
  int pad;
  int mxb, myb;

  // BMPの形式を記録するフラグ
  int isPM = FALSE;
  FILE *fp;

  WORD HEAD_bfType;
  DWORD HEAD_bfSize;
  WORD HEAD_bfReserved1;
  WORD HEAD_bfReserved2;
  DWORD HEAD_bf0ffBits;
  DWORD INFO_bfSize;
  Pixel palet[MAXCOLORS];
  Pixel setcolor;

  if((fp=fopen(fname,"rb"))==NULL){
    return -1;
  }

  /* BMPファイルは必ず'BM(0x4d42)'で始まる。それ以外の場合はBMPではないので、中止する */
  if(!freadWORD(&HEAD_bfType,fp)){
    errcode=-2;
    goto $ABORT;
  }

  if(HEAD_bfType != 0x4d42){
    errcode=-10;
    goto $ABORT;
  }

  /* ヘッダ部のサイズ(byte) */
  if(!freadDWORD(&HEAD_bfSize, fp)){
    errcode=-10;
    goto $ABORT;
  }

  //予約用領域(未使用)
  if(!freadWORD(&HEAD_bfReserved1, fp)){
    errcode=-10;
    goto $ABORT;
  }

  //予約用領域(未使用)  
  if(!freadWORD(&HEAD_bfReserved2, fp)){
    errcode=-10;
    goto $ABORT;
  }

  //オフセット
  if(!freadDWORD(&HEAD_bf0ffBits, fp)) {
    errcode=-10;
    goto $ABORT;
  }

  //ヘッダ部のサイズ
  if(!freadDWORD(&INFO_bfSize, fp)){
    errcode=-10;
    goto $ABORT;
  }

  //ヘッダ部のサイズが規定外ならばエラーとする
  if(INFO_bfSize == 40 || INFO_bfSize == 12){
    BMPInfo.biSize = INFO_bfSize;
    
    /* BITMAPCOREHEADER形式の場合 */
    if(INFO_bfSize == sizeof(BITMAPCOREHEADER)){
      WORD tmp;
      isPM = TRUE;
 
      // 画像の横幅
      if(!freadWORD(&tmp,fp)){
        errcode=-10;
        goto $ABORT;
      }
      BMPInfo.biWidth=tmp;

      // 画像の縦幅
      if(!freadWORD(&tmp,fp)){
        errcode-10;
        goto $ABORT;
      }
      if(!freadWORD(&(BMPInfo.biPlanes),fp)){
        errcode=-10;
        goto $ABORT;
      }
      if(!freadWORD(&(BMPInfo.biBitCount),fp)){
        errcode=-10;
        goto $ABORT;
      }
    }
    /* BITMAPINFOHEADER形式の場合 */
    else{
        /* 画像の横幅 */
        if(!freadDWORD(&(BMPInfo.biWidth), fp)){
            errcode =- 10;
            goto $ABORT;
        }

        /* 画像の縦幅 */
        if(!freadDWORD(&(BMPInfo.biHeight), fp)){
            errcode =- 10;
            goto $ABORT;
        }

        /* 画像のプレーン数 */
        if(!freadWORD(&(BMPInfo.biPlanes), fp)){
            errcode =- 10;
            goto $ABORT;
        }

        /* 1画素あたりのビット数 */
        if(!freadWORD(&(BMPInfo.biBitCount), fp)){
            errcode=-10;
            goto $ABORT;
        }
    }

    if(!isPM){
      if(!freadDWORD(&(BMPInfo.biCompression),fp)){
        errcode=-10;
        goto $ABORT;
      }
      if(!freadDWORD(&(BMPInfo.biSizeImage),fp)){
        errcode=-10;
        goto $ABORT;
      }
      if(!freadDWORD(&(BMPInfo.biXPelsPerMeter),fp)){
        errcode=-10;
        goto $ABORT;
      }if(!freadDWORD(&(BMPInfo.biYPelsPerMeter),fp)){
        errcode=-10;
        goto $ABORT;
      }
      if(!freadDWORD(&(BMPInfo.biClrUsed),fp)){
        errcode=-10;
        goto $ABORT;
      }if(!freadDWORD(&(BMPInfo.biClrImportant),fp)){
        errcode=-10;
        goto $ABORT;
      }
    }
  }
  else{
    errcode=-10;
    goto $ABORT;
  }

  mx = BMPInfo.biWidth;
  my = BMPInfo.biHeight;
  depth = BMPInfo.biBitCount;

  /* 256色, フルカラー以外はサポート外 */
  if(depth != 8 && depth != 24){
    errcode =- 3;
    goto $ABORT;
  }
  
  /* 非圧縮形式以外はサポート外 */
  if(BMPInfo.biCompression != BI_RGB){
    errcode =- 20;
    goto $ABORT;
  }
  if(BMPInfo.biClrUsed == 0){
    colors=count0fDIBColorEntries(BMPInfo.biBitCount);
  }
  else{
    colors = BMPInfo.biClrUsed;
  }


  if(!isPM){
    for(i=0;i<colors;i++){
      c=fgetc(fp);
      if(c==EOF){
        errcode=-10;
        goto $ABORT;
      }
      palet[i].g=c;

      c=fgetc(fp);
      if(c==EOF){
        errcode=-10;
        goto $ABORT;
      }
    }
  }else{
    for(i=0;i<colors;i++){
      c=fgetc(fp);
      if(c==EOF){
        errcode=-10;
        goto $ABORT;
      }
      palet[i].b=c;
      c=fgetc(fp);
      if(c==EOF){
        errcode=-10;
        goto $ABORT;
      }
      palet[i].r=c;
    }
  }

  *img = createImage(mx, my, 24);
  mxb = getDIBxmax(mx, depth);
  pad = mxb-mx*depth/8;

  for(y = my-1; y >= 0; y--){
    for(x = 0; x < mx; x++){
      if(depth == 8){
        c = fgetc(fp);
        if(c = EOF){
          errcode =- 20;
          goto $ABORT;
        }
        setcolor.r=palet[c].r;
        setcolor.g=palet[c].g;
        setcolor.b=palet[c].b;
      }
      else if(depth==24){
        c=fgetc(fp);
        if(c=EOF){
          errcode=-20;
          goto $ABORT;
        }
        setcolor.b=c;
        c=fgetc(fp);
        if(c=EOF){
          errcode=-20;
          goto $ABORT;
        }
        setcolor.g=c;
        c=fgetc(fp);
        if(c=EOF){
          errcode=-20;
          goto $ABORT;
        }
        setcolor.r=c;
      }
      setPixel(*img,x,y,&setcolor);
    }

    for(i=0;i<pad;i++){
      c=fgetc(fp);
      if(c==EOF){
        errcode=-20;
        goto $ABORT;
      }
    }
  }

 $ABORT:
  fclose(fp);
  return errcode;
}



int writeBMPfile(char *fname,ImageData *img)
{
  FILE *fp;
  BITMAPFILEHEADER bfn;
  int w,h,rw;
  int mxb,pad;
  int depth;
  int pbyte;
  int palsize;
  int x,y,i;
  int saveloop,saverest;
  int iBytes;
  unsigned int wsize;
  Pixel pix;

  w=img->width;
  h=img->height;
  depth=img->depth;


  if(depth!=24){
    goto $abort1;
  }

  if(depth == 24){
    pbyte = 1;
  }
  else{
    pbyte = depth / 8;
  }
  if(depth >= 24){
    palsize = 0;
  }
  else{
    palsize = 256;
  }


  rw=getDIBxmax(w, depth);

  bfn.bfType = 0x4d42;
  bfn.bfSize = 14+40+/* sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + */ palsize * 4 /* sizeof(RGBQUAD) */ + rw * h * pbyte;

  bfn.bfReserved1 = 0;
  bfn.bfReserved2 = 0;
  bfn.bf0ffBits = 14 + 40 + /* sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + */ palsize * 4 /* sizeof(RGBQUAD) */;


  if((fp=fopen(fname,"wb"))==NULL){

    goto $abort1;
  }

  fwriteWORD(bfn.bfType, fp);
  fwriteDWORD(bfn.bfSize, fp);
  fwriteWORD(bfn.bfReserved1, fp);
  fwriteWORD(bfn.bfReserved2, fp);
  fwriteDWORD(bfn.bf0ffBits, fp);
  fwriteDWORD(40 /* sizeof(BITMAPINFOHEADER) */, fp);
  fwriteDWORD(w, fp);   /* biWidth */
  fwriteDWORD(h, fp);   /* biHeight */
  fwriteWORD(1, fp);    /* biPlanes */
  fwriteWORD(depth, fp);    /*biBitCount */
  fwriteDWORD(BI_RGB, fp);  /* biCompression */
  fwriteDWORD(0, fp);       /* biSizeImage */
  fwriteDWORD(300, fp);     /* biXPelsPerMeter */
  fwriteDWORD(300, fp);     /* biYPelsPerMeter */
  fwriteDWORD(0, fp);       /* biClrUsed */
  fwriteDWORD(0, fp);       /* biClrImportant */

  /* 必要なパディングのサイズ */
  pad = rw - w * depth / 8;

  /* 画像データの書き出し */
  for(y=h-1;y>=0;y--){
    for(x=0;x<w;x++){
      getPixel(img, x, y, &pix);
      fputc(pix.b, fp);
      fputc(pix.g, fp);
      fputc(pix.r, fp);
    }
    for(i=0;i<pad;i++){
      fputc(0, fp);
    }
  }
  return 0;

 $abort1:
  return 1;
 $abort2:
  fclose(fp);
  return 1;
}
