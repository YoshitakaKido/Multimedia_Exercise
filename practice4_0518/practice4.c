#include<stdio.h>
#include"pgm.h"

/* 平均化して連結 */
void mean_to_contact(int n1, int n2, int n3, int n4);


int main(void)
{
    /* ファイル → 画像No.0 */
    load_image(0, "");
    
    /* ファイル → 画像No.1 */
    load_image(1, "");

    /* ファイル → 画像No.2 */
    load_image(2, "");

    /* 平均化して連結 */
    mean_to_contact(0, 1, 2, 3);

    /* 画像 No.3 → ファイル */
    save_image(3, "");

    return 0;
}


void mean_to_contact(int n1, int n2, int n3, int n4)
{
    int x, y, brightness_A, brightness_B, brightness_C, brightness_D, brightness_E;

    width[n4] = width[n1]*3;
    height[n4] = height[n1]*2;

    for(y = 0; y < height[n1]; y++)
    {
        for(x = 0; x < width[n1]; x++)
        {
            brightness_A = (image[n1][x][y] + image[n2][x][y]) / 2;
            brightness_B = (image[n1][x][y] + image[n2][x][y] + image[n3][x][y]) / 3;
            brightness_C = (image[n2][x][y] + image[n3][x][y]) / 2;
            brightness_D = (brightness_C + image[n1][x][y]) / 2;
            brightness_E = (brightness_A + image[n3][x][y]) / 2;
            
            image[n1][x][y] = brightness_B;
            image[n3][x][y] = brightness_E;
            image[n2][x][y] = brightness_D;
        }

    }

    int c = 0;

    for(y = 0; y < height[n3]; y++)
    {   
        for(x = 0; x < width[n3]; x++)
            image[n4][x][y] = image[n2][x][y];
    }

    for(y = 256; y < height[n4]; y++)
    {   
        for(x = 0; x < 256; x++)
        {
            image[n4][x][y] = image[n3][x][c];
        }
        c += 1;
    }

    int d = 0, e, f;

    for(y = 0; y < height[n4]; y++)
    {
        f = y * 2;
        for(x = 256; x < width[n4]; x++)
        {   
            if(x == 256)
                d = 0;
            else
                d += 1;

            image[n4][x+d][f] = image[n1][d][y];
            image[n4][x+d][f+1] = image[n1][d][y];
            image[n4][x+1+d][f] = image[n1][d][y];
            image[n4][x+1+d][f+1] = image[n1][d][y];
        }
    }
}

