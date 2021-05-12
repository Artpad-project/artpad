#include "DetectionCoin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int GaussPxl(int x, int y, int** Bitmap, double coefgauss2);

/* Default parameter :
 * coefgauss1 : 1
 * coefgauss2 : 2.5
 * harriscoef : 0.006
 * treshold : 130
 * */

void Detection(Image* BitMap,double coeffgauss1, double coeffgauss2, double harriscoef, int treshold)
{
    /* ------------------------ Initialisation ----------------------------- */

    int** BitMapGray = malloc(BitMap->height * sizeof (int*));
    int** BitMapA = malloc(BitMap->height * sizeof (int*));
    int** BitMapB = malloc(BitMap->height * sizeof (int*));
    int** BitMapC = malloc(BitMap->height * sizeof (int*));
    int** BitMapF = malloc(BitMap->height * sizeof (int*));
    int** BitMapFT = malloc(BitMap->height * sizeof (int*));
    int** BitMapSkip = malloc(BitMap->height * sizeof (int*));
    int** BitMapCoin = malloc(BitMap->height * sizeof (int*));

    for (int i = 0; i<BitMap->height; i++)
    {
        BitMapGray[i] = malloc(BitMap->width *sizeof(int));
        BitMapF[i] = malloc(BitMap->width *sizeof(int));
        BitMapFT[i] = malloc(BitMap->width *sizeof(int));
        BitMapA[i] = malloc(BitMap->width *sizeof(int));
        BitMapB[i] = malloc(BitMap->width *sizeof(int));
        BitMapC[i] = malloc(BitMap->width *sizeof(int));
        BitMapSkip[i] = malloc(BitMap->width *sizeof(int));
        BitMapCoin[i] = malloc(BitMap->width *sizeof(int));

        memset(BitMapGray[i],0,BitMap->width*sizeof(int));
        memset(BitMapF[i],0,BitMap->width*sizeof(int));
        memset(BitMapFT[i],0,BitMap->width*sizeof(int));
        memset(BitMapA[i],0,BitMap->width*sizeof(int));
        memset(BitMapB[i],0,BitMap->width*sizeof(int));
        memset(BitMapC[i],0,BitMap->width*sizeof(int));
        for(int j = 0 ; j<BitMap->width;j++ )BitMapSkip[i][j]=1;
        memset(BitMapCoin[i],0,BitMap->width*sizeof(int));

    }

    /* -------------------------- Grayscale ------------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            BitMapGray[i][j] =
                    (BitMap->pixels[j][i].red*0.2126 + BitMap->pixels[j][i].green*0.7152 + BitMap->pixels[j][i].blue*0.0722);
        }
    }

    /* ----------------------- Smoothing Image ---------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if (i > 1 && j > 1 && i < BitMap->height - 3 && j < BitMap->width - 3)
            {
                BitMapGray[i][j]=GaussPxl(i,j,BitMapGray,coeffgauss1);
            }
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
                BitMapA[i][j] = pow((BitMapGray[i+1][j] - BitMapGray[i-1][j])/2.0,2);
                BitMapB[i][j] = pow((BitMapGray[i][j+1] - BitMapGray[i][j-1])/2.0,2);
                BitMapC[i][j] = ((BitMapGray[i+1][j] - BitMapGray[i-1][j])/2.0)*((BitMapGray[i][j+1] - BitMapGray[i][j-1])/2.0);
            }
        }
    }

    /* -------------------------- Gauss Filter ----------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j) {
            if (i > 1 && j > 1 && i < BitMap->height - 3 && j < BitMap->width - 3)
            {
                BitMapA[i][j] = GaussPxl(i,j,BitMapA,coeffgauss2);
                BitMapB[i][j] = GaussPxl(i,j,BitMapB,coeffgauss2);
                BitMapC[i][j] = GaussPxl(i,j,BitMapC,coeffgauss2);
            }
        }
    }

    /* -------------------------- Final Matrix ----------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j)
        {
            BitMapFT[i][j] = (BitMapA[i][j] * BitMapB[i][j] - pow(BitMapC[i][j],2) - (harriscoef * pow((BitMapA[i][j] + BitMapB[i][j]),2)));
        }
    }

    /* ----------------------- SubPixel Accuracy -------------------------- */

    for (int i = 0; i < BitMap->height; ++i) {
        for (int j = 0; j < BitMap->width; ++j)
        {
            if(i == 0 || j == 0 || i == BitMap->height-1 || j == BitMap->width-1)
            {
                BitMapF[i][j]=BitMapFT[i][j];
            }
	    else
	    {
	      int DX = 0.5 * (BitMapFT[i+1][j] - BitMapFT[i-1][j]);
	      int DY = 0.5 * (BitMapFT[i][j+1] - BitMapFT[i][j-1]);
	      int DX2 = BitMapFT[i+1][j] + BitMapFT[i-1][j] - 2 * BitMapFT[i][j];
	      int DY2 = BitMapFT[i][j+1] + BitMapFT[i][j-1] - 2 * BitMapFT[i][j];
	      int DXY = 0.25 * (BitMapFT[i+1][j+1] + BitMapFT[i-1][j-1] - BitMapFT[i+1][j-1] - BitMapFT[i-1][j+1]);
	      int C = BitMapFT[i][j] + DX + DY + 0.5* (DX2 + DY2 + DXY);
	      BitMapF[i][j] = C;
	      if(C > treshold) BitMapSkip[i][j] = 0;
	    }
        }
    }

    /* ----------------------- Maximum Isolation -------------------------- */

    int radius = coeffgauss2*2;
    for (int i = radius; i < BitMap->height-radius ; ++i)
    {
        int j = radius;
        while (j < BitMap->width-radius && (BitMapSkip[i][j] ||  BitMapF[i][j-1] >= BitMapF[i][j]))
        {
            j++;
        }
        while (j < BitMap->width-radius)
        {
            while(j < BitMap->width-radius && (BitMapSkip[i][j] ||  BitMapF[i][j+1] >= BitMapF[i][j]))
            {
                j++;
            }
            if(j < BitMap->width-radius)
            {
                int p1 = j+2;
                while(p1 <= j + radius && BitMapF[i][p1] < BitMapF[i][j])
                {
                    BitMapSkip[i][p1] = 1;
                    p1++;
                }
                if(p1 > j + radius)
                {
                    int p2 = j-1;
                    while(p2 >= j-radius && BitMapF[i][p2] <= BitMapF[i][j])
                    {
                        p2--;
                    }
                    if(p2 < j-radius)
                    {
                        int k = i + radius;
                        int found = 0;
                        while(!found && k > i)
                        {
                            int l = j + radius;
                            while(!found && l >= j - radius)
                            {
                                if(BitMapF[k][l] > BitMapF[i][j]) found = 1;
                                else BitMapSkip[k][l] = 1;
                                l--;
                            }
                            k--;
                        }
                        k = i-radius;
                        while(!found && k < i)
                        {
                            int l = j - radius;
                            while(!found && l <= j + radius)
                            {
                                if(BitMapF[k][l] >= BitMapF[i][j]) found = 1;
                                l++;
                            }
                            k++;
                        }
                        if(!found) BitMapCoin[i][j]=1;
                    }
                }
                j = p1;
            }
        }
    }

    /* ----------------------------- Free --------------------------------- */

    for (int i = 0; i < BitMap->height; ++i)
    {
        for (int j = 0; j < BitMap->width; ++j)
        {
            printf("%d",BitMapCoin[i][j]);
        }
        printf("\n");
    }


    for (int i = 0; i < BitMap->height; ++i)
    {
        free(BitMapGray[i]);
        free(BitMapA[i]);
        free(BitMapB[i]);
        free(BitMapC[i]);
        free(BitMapF[i]);
        free(BitMapFT[i]);
        free(BitMapSkip[i]);
        free(BitMapCoin[i]);
    }
    free(BitMapGray);
    free(BitMapA);
    free(BitMapB);
    free(BitMapC);
    free(BitMapF);
    free(BitMapFT);
    free(BitMapSkip);
    free(BitMapCoin);
}


int GaussPxl(int x, int y,int** Bitmap,double coefgauss2)
{
    double gray_prime = 0;
    for(int i = x-2; i <= x+2 ; ++i )
    {
        int newx = i-x;
        for(int j = y-2; j <= y+2; ++j)
        {
            int newy = j-y;
            double coef = (1/(2*3.14*pow(coefgauss2,2)))*exp(-(pow(newx,2)+pow(newy,2))/(2*pow(coefgauss2,2)));
            gray_prime += Bitmap[i][j]*coef;
        }
    }
    return (int)gray_prime;
}
