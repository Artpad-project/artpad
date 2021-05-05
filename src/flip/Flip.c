#include "Flip.h"

void FlipHorizon(Image* Bitmap)
{
    for (int i = 0; i < Bitmap->height/2; ++i) {
        for (int j = 0; j < Bitmap->width; ++j) {
            Pixel tempo = Bitmap->pixels[j][i];
            Bitmap->pixels[j][i] = Bitmap->pixels[Bitmap->width-1-j][i];
            Bitmap->pixels[Bitmap->width-1-j][i] = tempo;
        }
    }
}


void FlipVertical(Image* Bitmap)
{
    for (int i = 0; i < Bitmap->height; ++i) {
        for (int j = 0; j < Bitmap->width/2; ++j) {
            Pixel tempo = Bitmap->pixels[j][i];
            Bitmap->pixels[j][i] = Bitmap->pixels[j][Bitmap->height-1-i];
            Bitmap->pixels[Bitmap->height-1-i][i] = tempo;
        }
    }
}