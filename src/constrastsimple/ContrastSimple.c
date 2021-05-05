#include "ContrastSimple.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int Cap(double x);

int Constrast(Image* Bitmap, double coefcontrast, double brightness)
{

    for (int i = 0; i < Bitmap->height ; ++i)
    {
        for(int j = 0; j < Bitmap->width ; ++j)
        {
            if(i >= 0 && j>=0 && i<=Bitmap->height-1 && j<=Bitmap->width-1)
            {
                Bitmap->pixels[i][j].red =  Cap(Bitmap->pixels[i][j].red * coefcontrast + brightness);
                Bitmap->pixels[i][j].green = Cap(Bitmap->pixels[i][j].green * coefcontrast + brightness);
                Bitmap->pixels[i][j].blue = Cap(Bitmap->pixels[i][j].blue * coefcontrast + brightness);
            }
        }
    }

    return 0;
}

int Cap(double x)
{
    if(x<0)return 0;
    if(x>255)return 0;
    return (int)x;
}
