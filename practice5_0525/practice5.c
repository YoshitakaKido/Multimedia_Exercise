#include<stdio.h>
#include"pgm.h"
void Linear_trans(int m, int n);


int main(void)
{
    load_image(0, "");
    Linear_trans(0, 1);
    save_image(1, "");
    return 0;
}


void Linear_trans(int m, int n)
{
    int x, y;
    int max= 0, min = 255;
    width[n] = width[m]; height[n] = height[m];

    for(y = 0; y < height[m]; y++)
    {
        for(x = 0; x < width[m]; x++)
        {
            /* 最大階調値を求める */
            if(image[m][x][y] > max) max = image[m][x][y];

            /* 最小階調値を求める */
            if(min > image[m][x][y]) min = image[m][x][y];
        }
    }

    /* 線形変換 */
    for(y = 0; y < height[m]; y++)
    {
        for(x = 0; x < width[m]; x++)
        { 
            image[n][x][y] = (int)(((double)(image[m][x][y]-min)/(max-min))*255);
        }
    }
}
