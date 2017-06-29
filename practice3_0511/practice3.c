#include<stdio.h>
#include"pgm.h"

/* 画像 No.1 を180度回転させて画像 No.2へ */
void large(int n1, int n2);


int main(void)
{
    /* ファイル → 画像No.0 */
    load_image(0, "");

    /* No.0 を横２倍縦３倍にしてNo.1へ */
    large(0, 1);

    /* 画像 No.1 → ファイル */
    save_image(1, "");

    return 0;
}

/* 画像 No.n1を180度させて画像 No.n2へ */
void large(int n1, int n2)
{
    int x, y, c = 0, d, i, state;


    width[n2] = width[n1]*3; //横の長さを３倍に
    height[n2] = height[n1]*3; //縦の長さを３倍に


    for (y=0; y < height[n1]; y++)
    {
        d = y * 2; // 出力画像に対する縦の長さの調節
        for (x=0; x < width[n1]; x++)
        {
            c = x * 2; // 出力画像に対する横の長さの調節
            image[n2][c][y+d] = image[n1][x][y];
            image[n2][1+c][y+d] = image[n1][x][y];
            image[n2][c][y+1+d] = image[n1][x][y];
            image[n2][1+c][y+1+d] = image[n1][x][y];
            image[n2][c][y+2+d] = image[n1][x][y];
            image[n2][1+c][y+2+d] = image[n1][x][y];
         }
    }
    for (i=0; i < 3; i++)
    {
        if (i == 0) d = 0;
        else    d += height[n1];

        state = c+2;
        for (y=0; y < height[n1]; y++)
        {
            for (x=0; x < width[n1]; x++)
            {
                image[n2][x+state][y+d] = image[n1][x][y];
            }
        }
    }
}
