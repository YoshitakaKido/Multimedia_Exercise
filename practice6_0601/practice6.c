#include<stdio.h>
#include"ppm.h"


void reverse(int n1, int n2);

int main(void)
{
    load_color_image(0, "");
    
    reverse(0, 1);

    save_color_image(1, "");

    return 0;
}


void reverse(int n1, int n2)
{
    int x, y, col;
    width[n2] = width[n1];
    height[n2] = height[n1];

    for(y = 0; y < height[n1]; y++)
    {
        for(x = 0; x < width[n1]; x++)
        {
            for(col = 0; col < 3; col++)
            {   
                image[n2][x][y][col] = 255 - image[n1][x][y][col];
            }
        }
    }   
}
