#include "KLT.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Gray(int i, int j,Image* BitMap);
double CoefGaussien(int x, int y);

void KLT(Image* Bitmap1, Image* Bitmap2, int radius, Coord* ListPoint, int nbPoint)
{
    int** dIm1X = malloc((2*radius+1) *sizeof(int*));
    int** dIm1Y = malloc((2*radius+1) *sizeof(int*));
    int** dIm1XT = malloc((2*radius+1) *sizeof(int*));
    int** dIm1YT = malloc((2*radius+1) *sizeof(int*));
    Coord* Deplacement  = malloc(nbPoint * sizeof(Coord));
    for(int i  = 0; i < 2*radius+1; i++)
    {
        dIm1X[i] = malloc((2*radius +1)* sizeof(int));
        dIm1Y[i] = malloc((2*radius +1)* sizeof(int));
        dIm1XT[i] = malloc((2*radius +1)* sizeof(int));
        dIm1YT[i] = malloc((2*radius +1)* sizeof(int));
    }


    for (int m = 0; m < nbPoint; ++m) {
        int i = ListPoint[m].x;
        int j = ListPoint[m].y;
/* --------------------------- Init Local ---------------------------------- */

        for(int x = i-radius; x <= i+radius ; ++x )
        {
            int newx = x - i + radius;
            for(int y = j-radius; y <= j+radius; ++y)
            {
                int newy = y - j + radius;
                dIm1X[newx][newy] = (Gray(x,y+1,Bitmap1) - Gray(x,y-1,Bitmap1))/2;
                dIm1Y[newx][newy] = (Gray(x+1,y,Bitmap1) - Gray(x-1,y,Bitmap1))/2;
            }
        }

        for (int k = 0; k < 2*radius+1 ; ++k) {
            for (int l = 0; l < 2*radius+1 ; ++l) {
                if((k*(2*radius+1)+l)%(2*radius+2) != 0)
                {
                    dIm1XT[k][l] = dIm1X[(2*radius)-k][(2*radius)-l];
                    dIm1YT[k][l] = dIm1Y[(2*radius)-k][(2*radius)-l];
                }
                else
                {
                    dIm1XT[k][l] = dIm1X[k][l];
                    dIm1YT[k][l] = dIm1Y[k][l];
                }
            }
        }

/* -------------------------- Determination -------------------------------- */
        int sdxdt = 0;
        int sdydt = 0;
        int sdxt = 0;
        int sdyt = 0;
        for(int x = i-radius; x <= i+radius ; ++x )
        {
            int newx = x - i + radius;
            for(int y = j-radius; y <= j+radius; ++y)
            {
                int newy = y - j + radius;
                sdxdt += (Gray(x,y,Bitmap2) - Gray(x,y,Bitmap1))*dIm1X[newx][newy] ;
                sdydt += (Gray(x,y,Bitmap2) - Gray(x,y,Bitmap1))*dIm1Y[newx][newy] ;
                sdxt += dIm1X[newx][newy] * dIm1XT[newx][newy] ;
                sdyt += dIm1Y[newx][newy] * dIm1YT[newx][newy] ;
            }
        }

        Coord deplacement = {
                .x = sdxdt/sdxt,
                .y = sdydt/sdyt,
        };
        Deplacement[m] = deplacement;
    }

/* ------------------------------ Free ------------------------------------ */

    for (int m = 0; m < nbPoint; ++m) {
        Pixel newpix =
                {.red = 255,
                        .blue = 255,
                        .green = 255,
                        .alpha = 255,
                };
        Bitmap1->pixels[ListPoint[m].y][ListPoint[m].x] = newpix;
        Bitmap2->pixels[ListPoint[m].y+Deplacement[m].y][ListPoint[m].x+Deplacement[m].x] = newpix;
        printf("[%i][%i] : %i , %i\n",ListPoint[m].x,ListPoint[m].y,Deplacement[m].x,Deplacement[m].y);
    }

    for (int i = 0; i < 2*radius+1; ++i) {
        free(dIm1X[i]);
        free(dIm1Y[i]);
        free(dIm1XT[i]);
        free(dIm1YT[i]);
    }
    free(dIm1Y);
    free(dIm1XT);
    free(dIm1X);
    free(dIm1YT);
    free(Deplacement);
}


int Gray(int i, int j,Image* BitMap)
{
    int gray = BitMap->pixels[j][i].red*0.2126 + BitMap->pixels[j][i].green*0.7152 + BitMap->pixels[j][i].blue*0.0722;
    return gray;
}


