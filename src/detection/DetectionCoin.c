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
        BitMapGray[i] = malloc(BitMap->width *sizeof(int));
        BitMapF[i] = malloc(BitMap->width *sizeof(int));
        BitMapA[i] = malloc(BitMap->width *sizeof(int));
        BitMapB[i] = malloc(BitMap->width *sizeof(int));
        BitMapC[i] = malloc(BitMap->width *sizeof(int));

        memset(BitMapGray[i],0,BitMap->width*sizeof(int));
        memset(BitMapF[i],0,BitMap->width*sizeof(int));
        memset(BitMapA[i],0,BitMap->width*sizeof(int));
        memset(BitMapB[i],0,BitMap->width*sizeof(int));
        memset(BitMapC[i],0,BitMap->width*sizeof(int));

    }

    /* -------------------------- Grayscale ------------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            BitMapGray[i][j] = (int)
                    (BitMap->pixels[j][i].red*0.2126 + BitMap->pixels[j][i].green*0.7152 + BitMap->pixels[j][i].blue*0.0722);
        }
    }

    /* --------------------------- Matrix --------------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if(i == 0 || j == 0 || i == BitMap->height-1 || j == BitMap->width-1)
            {
                BitMapA[i][j] = 0;
                BitMapB[i][j] = 0;
                BitMapC[i][j] = 0;
            }
            else
            {
               BitMapA[i][j] = (int)(pow(BitMapGray[i+1][j] - BitMapGray[i-1][j],2));
               BitMapB[i][j] = (int)(pow(BitMapGray[i][j+1] - BitMapGray[i][j-1],2));
               BitMapC[i][j] = (BitMapGray[i+1][j] - BitMapGray[i-1][j])*(BitMapGray[i][j+1] - BitMapGray[i][j-1]);
            }
        }
    }

    /* -------------------------- Gauss Filter ----------------------------- */

    /*for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if (i > 1 && j > 1 && i < BitMap->height - 3 && j < BitMap->width - 3)
            {
                BitMapA[i][j] = GaussPxl(i,j,BitMapA);
                BitMapB[i][j] = GaussPxl(i,j,BitMapB);
                BitMapC[i][j] = GaussPxl(i,j,BitMapC);
            }
        }
	}*/

    /* -------------------------- Final Matrix ----------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if(i == 0 || j == 0 || i == BitMap->height-1 || j == BitMap->width-1)
            {
                BitMapF[i][j] = 0;
            }
            else
            {
	      int C = ABS(BitMapA[i][j] * BitMapB[i][j] - pow(BitMapC[i][j],2) - 0.05 * pow((BitMapA[i][j] + BitMapB[i][j]),2));
	      //printf("%d ",C);
                if(C > 25000)
                {
                    BitMapF[i][j] = 1;
                }
                else
                {
                    BitMapF[i][j] = 0;
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
            printf("%d",BitMapF[i][j]);
        }
        printf("\n");
	}
}


int GaussPxl(int x, int y,int** Bitmap)
{
    double gray_prime = 0;
    int convo = 0;
    for(int i = x-2; i <= x+2 ; ++i )
    {
        for(int j = y-2; j <= y+2; ++j)
        {
            double coef = GaussMatrix[convo];
            gray_prime += Bitmap[i][j]*coef;
            ++convo;
        }
    }
    gray_prime *= 0.004;
    return (int)gray_prime;
}


