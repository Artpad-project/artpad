#include "../../include/Rotation.h"
#include <stdlib.h>
#include <math.h>


int RotatePxlX(float angle, int x, int xc, int y, int yc);
int RotatePxlY(float angle, int x, int xc, int y, int yc);

int Rotate(Image* BitMap, float angle)
{
    angle = angle *(float)(3.1415/180);
    int xc = BitMap->width/2;
    int yc = BitMap->height/2;
    Pixel** BitMapRotate = malloc(BitMap->height * sizeof(Pixel*));
    for (int i = 0; i<BitMap->height; i++)
    {
      BitMapRotate[i] = malloc(BitMap->width * sizeof(Pixel)); 
      memset(BitMapRotate[i],0,BitMap->width*sizeof(Pixel));
     
    }

    for (int i = 0; i < BitMap->height ; ++i)
    {
        for(int j = 0; j < BitMap->width ; ++j)
        {
            int posx = RotatePxlX(angle,j,xc,i,yc);
            int posy = RotatePxlY(angle,j,xc,i,yc);
            if(posy < BitMap->height && posx < BitMap->width && posx >= 0 && posy >= 0 )
            {
                BitMapRotate[i][j] = BitMap->pixels[posx][posy];

            }
        }
    }
    
    for (int i = 0; i < BitMap->height; ++i)
    {
        for (int j = 0; j < BitMap->width; ++j)
        {
            BitMap->pixels[j][i] = BitMapRotate[i][j];
        }
	free(BitMapRotate[i]);
    }
    free(BitMapRotate);
    return 0;
}


int RotatePxlX(float angle, int x, int xc, int y, int yc)
{
    float a1 = (float)(x - xc) * cosf( -angle );
    float a2 = (float)(y - yc) * sinf( -angle );
    return ((int)xc + (int)a1 - (int)a2);
}

int RotatePxlY(float angle, int x, int xc, int y, int yc)
{
    float a1 = (float)(x - xc) * (float)sinf( -angle );
    float a2 = (float)(y - yc) * (float)cosf( -angle );
    return (int)yc + (int)a1 + (int)a2;
}
