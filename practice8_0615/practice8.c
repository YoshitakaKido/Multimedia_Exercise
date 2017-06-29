#include<stdio.h>
#include"ppm.h"

long int hist[256][3];
void make_histogram_image(int m, int h, int n);
void separate(int n1, int n2);

int main(void)
{
    load_color_image(0, "");    
    make_histogram_image(0, 256, 1);
    save_color_image(1, "");

    return 0;
}


void make_histogram_image(int m, int h, int n)
{
    int i, j, x, y, col;
    long int max[3] = {0};
    long int max_mix = 0;
    int takasa;

    width[n] = 256*4;
    height[n] = 256*3;
    
    /* ヒストグラム作成 */
    for(j = 0; j < 3; j++){
        for(i = 0; i < 256; i++)
            hist[i][j]= 0;
    }
    for(i = 0; i< 3; i++){
        for(y = 0; y < height[m]; y++){
            for(x = 0; x < width[m]; x++){
                hist[ image[m][x][y][i] ][i]++;
            }
        }    
    }


    /* 縦3連結 */
    for(col = 0; col < 3; col++){
        for(i = 0; i < 256; i++)
        {
            if(hist[i][col] > max[col])
                max[col] = hist[i][col];
        }
        for(x = 0; x < 256; x++)
        {
            takasa = (int)(h / (double)max[col] * hist[x][col]);
            if(takasa > h)
                takasa = h;

            for(y = 0; y < h; y++)
            {   
                if(col == 0){
                    if(y < takasa)
                        image[n][x][h-1-y][col] = 255;
                    else
                        image[n][x][h-1-y][col] = 0;
                }
                else if(col == 1){
                    if(y < takasa)
                        image[n][x][h-1-y+256][col] = 255;
                    else
                        image[n][x][h-1-y+256][col] = 0;
                }
                else{
                    if(y < takasa)
                        image[n][x][h-1-y+512][col] = 255;
                    else
                        image[n][x][h-1-y+512][col] = 0;
                }
            }
        }
    }
    /* 拡大フィーバー */
    for(col = 0; col < 3; col++){
        for(i = 0; i < 256; i++){
            if(hist[i][col]*3 > max_mix)
                max_mix = hist[i][col]*3;
        }

        for(x = 256; x < 256*4; x++){
            takasa = (int)(h*3 / (double)max_mix * hist[(x-256)/3][col])*3;
            if(takasa > 256*3)
                takasa = 256*3;

            for(y = 0; y < 256*3; y++){   
                if(y < takasa)
                    image[n][x][(h*3-1-y)][col] = 255;
                else
                    image[n][x][(h*3-1-y)][col] = 0;
            }
        }
    }
}   
