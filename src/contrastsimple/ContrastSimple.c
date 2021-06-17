#include "../../include/ContrastSimple.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int Cap3(double x);

void Contrast(Image* Bitmap, double coefcontrast, double brightness)
{

    for (int i = 0; i < Bitmap->height ; ++i)
    {
        for(int j = 0; j < Bitmap->width ; ++j)
        {
            
	  Bitmap->pixels[j][i].red =  Cap3(Bitmap->pixels[j][i].red * coefcontrast + brightness);
	  Bitmap->pixels[j][i].green = Cap3(Bitmap->pixels[j][i].green * coefcontrast + brightness);
	  Bitmap->pixels[j][i].blue = Cap3(Bitmap->pixels[j][i].blue * coefcontrast + brightness);
           
        }
    }

}

int Cap3(double x)
{
    if(x<0)return 0;
    if(x>255)return 255;
    return (int)x;
}
