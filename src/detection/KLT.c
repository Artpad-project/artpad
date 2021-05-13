#include "KLT.h"
#include <stdio.h>
#include <stdlib.h>

int Gray(int i, int j,Image* BitMap);

void KLT(Image* Bitmap1, Image* Bitmap2, int radius, Coord* ListPoint, int nbPoint)
{
  int** dIm1X = malloc((2*radius+1) *sizeof(int*));
  int** dIm1Y = malloc((2*radius+1) *sizeof(int*));
  int** dIm1XT = malloc((2*radius+1) *sizeof(int*));
  int** dIm1YT = malloc((2*radius+1) *sizeof(int*));
  Coord* Deplacement  = malloc(nbPoint * sizeof(Deplacement));
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
                dIm1X[newx][newy] = (Gray(x,y-1,Bitmap1) - Gray(x,y+1,Bitmap1))/2;
                dIm1Y[newx][newy] = (Gray(x-1,y,Bitmap1) - Gray(x+1,y,Bitmap1))/2;
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
        int spx = 0;
        int spy = 0;
        int spxt = 0;
        int spyt = 0;
        for(int x = i-radius; x <= i+radius ; ++x )
        {
            int newx = x - i + radius;
            for(int y = j-radius; y <= j+radius; ++y)
            {
                int newy = y - j + radius;
		//printf("%d %d\n",dIm1X[newx][newy],dIm1Y[newx][newy]);
                spx += (Gray(x,y,Bitmap2) - Gray(x,y,Bitmap1))*dIm1X[newx][newy];
                spy += (Gray(x,y,Bitmap2) - Gray(x,y,Bitmap1))*dIm1Y[newx][newy];
                spxt += dIm1XT[newx][newy] * dIm1X[newx][newy];
                spyt += dIm1YT[newx][newy] * dIm1Y[newx][newy];
            }
        }

        Coord deplacement = {
	  .x = spxt != 0 ? spx / spxt : 0,
	  .y = spyt != 0 ? spy / spyt : 0,
        };
        Deplacement[m] = deplacement;
	ListPoint[m].x += deplacement.x;
	ListPoint[m].y+= deplacement.y;
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
