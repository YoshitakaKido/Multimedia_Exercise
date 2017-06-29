#include<stdio.h>
#include"ppm.h"

long int hist[256][3];
void auto_image(int m, int h, int n);

int main(void)
{
    load_color_image(0, "");    
    auto_image(0, 256, 1);
    save_color_image(1, "");

    return 0;
}


void auto_image(int m, int h, int n)
{
    int i, j, x, y, col;
    long int max[3] = {0};
    long int max_mix = 0;
    int takasa;
    
    width[n] = 256;
    height[n] = 256;
   
    for(y = 0; y < 256; y++){
        for(x = 0; x < 256; x++){
            if(image[m][x][y][1] > 50)
                if(image[m][x][y][0] > 181 || image[m][x][y][2] > 181){
                    for(col = 0; col < 3; col++)
                        image[n][x][y][col] = 255;
                    }
                else
                    for(col = 0; col < 3; col++)
                        image[n][x][y][col] = image[m][x][y][col];
            else
                for(col = 0; col < 3; col++)
                    image[n][x][y][col] = 255;
        }
    }

}
