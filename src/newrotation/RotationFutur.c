#include "RotationFutur.h"
#include <stdlib.h>
#include <math.h>

int RotatePxlX_Y(float angle, int x, int xc, int z, int zc);
int RotatePxlX_Z(float angle, int x, int xc, int y, int yc);
int RotatePxlY_X(float angle, int y, int yc, int z, int zc);
int RotatePxlY_Z(float angle, int x, int xc, int y, int yc);
int RotatePxlZ_X(float angle, int y, int yc, int z, int zc);
int RotatePxlZ_Y(float angle, int x, int xc, int z, int zc);


int Rotate(Image* BitMap, float anglex, float angley, float anglez)
{
    anglex = anglex *(float)(3.1415/180);
    angley = angley *(float)(3.1415/180);
    anglez = anglez *(float)(3.1415/180);

    int xc = BitMap->width/2;
    int yc = BitMap->height/2;
    int zc = BitMap->width/2;

    Pixel*** BitMapRotate = malloc(BitMap->height * sizeof (Pixel*));
    for (int i = 0; i<BitMap->height; i++)
    {
        BitMapRotate[i] = malloc(BitMap->width * sizeof (Pixel**));
        for (int j = 0; j<BitMap->width; j++)
        {
            BitMapRotate[i][j] = malloc(BitMap->width * sizeof (Pixel***));
            //memset(BitMapRotate[i][j],0,BitMap->width*sizeof(Pixel));
        }
    }
    for (int i = 0; i < BitMap->height; ++i)
    {
        for (int j = 0; j < BitMap->width; ++j)
        {
            for (int v = 0; j < BitMap->width; ++v){
                BitMapRotate[i][j][v].red = 0;
                BitMapRotate[i][j][v].green = 0;
                BitMapRotate[i][j][v].blue = 0;
            }
        }
    }


    for (int i = 0; i < BitMap->height ; ++i)
    {
        for(int j = 0; j < BitMap->width ; ++j)
        {
            /* Rotation sur axe X */

            int posy1 = RotatePxlY_X(anglex,i,yc,zc,zc);
            int posz1 = RotatePxlZ_X(anglex,i,yc,zc,zc);

            /* Rotation sur axe Y */

            int posx1 = RotatePxlX_Y(angley,j,xc,posz1,zc);
            int posz = RotatePxlZ_Y(angley,j,xc,posz1,zc);

            /* Rotation sur axe Z */

            int posx = RotatePxlX_Z(anglez,posx1,xc,posy1,yc);
            int posy = RotatePxlY_Z(anglez,posx1,xc,posy1,yc);


            if(posy < BitMap->height && posx < BitMap->width && posz < BitMap->width && posz >= 0 && posx >= 0 && posy >= 0 )
            {
                BitMapRotate[posy][posx][posz].red = BitMap->pixels[i][j].red;
                BitMapRotate[posy][posx][posz].green = BitMap->pixels[i][j].green;
                BitMapRotate[posy][posx][posz].blue = BitMap->pixels[i][j].blue;
            }
        }
    }


    for (int i = 0; i < BitMap->height; ++i)
    {
        for (int j = 0; j < BitMap->width; ++j)
        {
            int k = 0;
            while(k < BitMap->width)
            {
                if(BitMapRotate[i][j][k].red || BitMapRotate[i][j][k].green || BitMapRotate[i][j][k].blue) break;
                ++k;
            }
            BitMap->pixels[i][j].red = BitMapRotate[i][j][k].red;
            BitMap->pixels[i][j].green = BitMapRotate[i][j][k].green;
            BitMap->pixels[i][j].blue = BitMapRotate[i][j][k].blue;
            free(BitMapRotate[i][j]);
        }
        free(BitMapRotate[i]);
    }
    free(BitMapRotate);
    return 0;
}


int RotatePxlX_Y(float angle, int x, int xc, int z, int zc)
{
    float a1 = (float)(x - xc) * (float)cosf( angle );
    float a2 = (float)(z - zc) * (float)sinf( angle );
    int newx =  ((int)xc + (int)a1 + (int)a2);
    return newx;
}

int RotatePxlX_Z(float angle, int x, int xc, int y, int yc)
{
    float a1 = (float)(x - xc) * (float)cosf( angle );
    float a2 = (float)(y - yc) * (float)sinf( angle );
    int newx =  ((int)xc + (int)a1 - (int)a2);
    return newx;
}

int RotatePxlY_X(float angle, int y, int yc,int z, int zc)
{
    float a1 = (float)(y - yc) * (float)cosf( angle );
    float a2 = (float)(z - zc) * (float)sinf( angle );
    int newy =  ((int)yc + (int)a1 - (int)a2);
    return newy;
}

int RotatePxlY_Z(float angle, int x, int xc, int y, int yc)
{
    float a1 = (float)(x - xc) * (float)sinf( angle );
    float a2 = (float)(y - yc) * (float)cosf( angle );
    int newy =  ((int)yc + (int)a1 - (int)a2);
    return newy;
}

int RotatePxlZ_X(float angle, int y, int yc, int z, int zc)
{
    float a1 = (float)(y - yc) * (float)sinf( angle );
    float a2 = (float)(z - zc) * (float)cosf( angle );
    int newz =  ((int)zc + (int)a1 + (int)a2);
    return newz;
}

int RotatePxlZ_Y(float angle, int x, int xc, int z, int zc)
{
    float a1 = (float)(x - xc) * (float)sinf( angle );
    float a2 = (float)(z - zc) * (float)cosf( angle );
    int newz =  ((int)zc - (int)a1 + (int)a2);
    return newz;
}