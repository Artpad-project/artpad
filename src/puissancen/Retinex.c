#include "Retinex.h"
#include <stdio.h>
#include <math.h>

struct Pixel{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Image{
    int width, height;
    struct Pixel** pixels;
};

unsigned char RetinexPxl(unsigned char canal, size_t x, size_t y, int fact);

struct Image Retinex(struct Image Bitmap)
{
    int fact = (((Bitmap.height) < (Bitmap.width)) ? (Bitmap.height) : (Bitmap.width));
    for (int x = 0; x < Bitmap.height ; ++x)
    {
        for (int y = 0; y < Bitmap.width ; ++y)
        {
            Bitmap.pixels[x][y].red = RetinexPxl(Bitmap.pixels[x][y].red,x,y, fact);
            Bitmap.pixels[x][y].green = RetinexPxl(Bitmap.pixels[x][y].green,x,y, fact);
            Bitmap.pixels[x][y].blue = RetinexPxl(Bitmap.pixels[x][y].blue,x,y, fact);
        }
    }
    return Bitmap;
}

unsigned char RetinexPxl(unsigned char canal, size_t x, size_t y, int fact)
        {
            canal = !canal ? 1 : canal;
            double a1 = log(canal);
            double gauss = exp(-(pow(x,2)+pow(y,2))/pow(fact,2));
            double a2 = log(gauss*canal);
            return (unsigned char)(a1 - a2);
        }