#include<stdio.h>
#include"pgm.h"
long int hist[256];
void make_histogram(int n);
void make_histogram_image(int h, int n);


int main(void)
{
    load_image(0, "");
    make_histogram(0);
    /* make_histogram_image(256, 1); */
    save_image(1, "");
    return 0;
}


void make_histogram(int n)
{
    int i, x, y;

    for(i = 0; i < 256; i++) 
        hist[i] = 0;
    
    for(y = 0; y < height[n]; y++)
    {
        for(x = 0; x < width[n]; x++)
        {
            hist[ image[n][x][y] ]++;
        }
    }
    make_histogram_image(256, 1);
}


void make_histogram_image(int h, int n)
{
    int i, x, y;
    long int max;
    int takasa;

    width[n] = 256;
    height[n] = h;
    max = 0;

    for(i = 0; i < 256; i++)
    {
        if(hist[i] > max)
            max = hist[i];
    }

    for(x = 0; x < width[n]; x++)
    {
        takasa = (int)(h / (double)max * hist[x]);
        if(takasa > h)
            takasa = h;

        for(y = 0; y < h; y++)
        {
            if(y < takasa)
                image[n][x][h-1-y] = 0;
            else
                image[n][x][h-1-y] = 255;
        }
    }
}
