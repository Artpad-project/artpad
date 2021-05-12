#include "DetectionCoin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

double GaussMatrix[25] = {0.03,0.16,5.98,0.16,0.03,0.16,7.7,27.8,7.7,0.16,5.98,27.8,100,27.8,5.98,0.16,7.7,27.8,7.7,0.16,0.03,0.16,5.98,0.16,0.03};
int GaussPxl(int x, int y,int** Bitmap);

void Detection(Image* BitMap)
{
    /* ------------------------ Initialisation ----------------------------- */

    int** BitMapGray = malloc(BitMap->height * sizeof (int*));
    int** BitMapA = malloc(BitMap->height * sizeof (int*));
    int** BitMapB = malloc(BitMap->height * sizeof (int*));
    int** BitMapC = malloc(BitMap->height * sizeof (int*));
    int** BitMapF = malloc(BitMap->height * sizeof (int*));

    for (int i = 0; i<BitMap->height; i++)
    {
        BitMapGray[i] = malloc(BitMap->width * sizeof *BitMapGray[i]);
        BitMapF[i] = malloc(BitMap->width * sizeof *BitMapF[i]);
        BitMapA[i] = malloc(BitMap->width * sizeof *BitMapA[i]);
        BitMapB[i] = malloc(BitMap->width * sizeof *BitMapB[i]);
        BitMapC[i] = malloc(BitMap->width * sizeof *BitMapC[i]);

        memset(BitMapGray[i],0,BitMap->width);
        memset(BitMapF[i],0,BitMap->width);
        memset(BitMapA[i],0,BitMap->width);
        memset(BitMapB[i],0,BitMap->width);
        memset(BitMapC[i],0,BitMap->width);

    }

    /* -------------------------- Grayscale ------------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            BitMapGray[j][i] = (int)
                    (BitMap->pixels[j][i].red*0.2126 + BitMap->pixels[j][i].green*0.7152 + BitMap->pixels[j][i].blue*0.0722);
        }
    }

    /* --------------------------- Matrix --------------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if(i == 0 || j == 0 || i == BitMap->height-1 || j == BitMap->width-1)
            {
                BitMapA[j][i] = 0;
                BitMapB[j][i] = 0;
                BitMapC[j][i] = 0;
            }
            else
            {
               BitMapA[j][i] = (int)(pow(BitMapGray[j+1][i] - BitMapGray[j-1][i],2));
               BitMapB[j][i] = (int)(pow(BitMapGray[j][i+1] - BitMapGray[j][i-1],2));
               BitMapC[j][i] = (BitMapGray[j+1][i] - BitMapGray[j-1][i])*(BitMapGray[j][i+1] - BitMapGray[j][i-1]);
            }
        }
    }

    /* -------------------------- Gauss Filter ----------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if (i > 1 && j > 1 && i < BitMap->height - 3 && j < BitMap->width - 3)
            {
                BitMapA[j][i] = GaussPxl(i,j,BitMapA);
                BitMapB[j][i] = GaussPxl(i,j,BitMapB);
                BitMapC[j][i] = GaussPxl(i,j,BitMapC);
            }
        }
    }

    /* -------------------------- Final Matrix ----------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if(i == 0 || j == 0 || i == BitMap->height-1 || j == BitMap->width-1)
            {
                BitMapF[j][i] = 0;
            }
            else
            {
                int C = (int)((double)(BitMapA[j][i] * BitMapB[j][i] - BitMapC[j][i])*2 - 0.05 * (BitMapA[j][i] + BitMapB[j][i]) *2);
                if(C > 106)
                {
                    BitMapF[j][i] = C;
                }
                else
                {
                    BitMapF[j][i] = 0;
                }
            }

        }
    }

    for (int i = 0; i < BitMap->height; ++i)
    {
        free(BitMapGray[i]);
        free(BitMapA[i]);
        free(BitMapB[i]);
        free(BitMapC[i]);
    }
    free(BitMapGray);
    free(BitMapA);
    free(BitMapB);
    free(BitMapC);

    for (int i = 0; i < BitMap->height; ++i)
    {
        for (int j = 0; j < BitMap->width; ++j)
        {
            printf("%d ",BitMapF[j][i]);
        }
        printf("\n");
    }
}


int GaussPxl(int x, int y,int** Bitmap)
{
    double gray_prime = 0;
    int convo = 0;
    for(int i = x-3; i <= x+3 ; ++i )
    {
        for(int j = y-3; j <= y+3; ++j)
        {
            double coef = GaussMatrix[convo];
            gray_prime += Bitmap[j][i]*coef;
            ++convo;
        }
    }
    gray_prime *= 0.004;
    return (int)gray_prime;
}
