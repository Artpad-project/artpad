#include "Saturation.h"
#include <math.h>

double max(double x, double y);
double min(double x, double y);
double Hue(double red, double green, double blue, double cmax, double delta);
double absolu(double x);
void Saturation(Image* Bitmap, int x, int y,double sat);
int Cap(double x);
double CapSat(double x);

void SaturationAbsolue(Image* Bitmap, double Indice)
{
  for(double x = 0; x < Bitmap->height; ++x)
    {
      for(double y = 0; y < Bitmap->width; ++y)
	{
	  Saturation(Bitmap, y, x, Indice);
	}
    }

}


void Saturation(Image* Bitmap, int x, int y,double sat)
{
  
    double nrouge = Bitmap->pixels[x][y].red / 255.0;
    double nvert = Bitmap->pixels[x][y].green / 255.0;
    double nbleu = Bitmap->pixels[x][y].blue / 255.0;

    double cmax = max(nbleu, max(nrouge,nvert));
    double cmin = min(nbleu, min(nvert,nrouge));
    double delta = cmax - cmin;

    double hue = delta == 0 ? 0 : Hue(nrouge,nvert,nbleu,cmax,delta) ;
    double luminance = (cmax + cmin)/2.0;
    double saturation = delta == 0 ? 0 : CapSat(delta /(1-absolu(2*luminance-1))) ;

    if(sat > 0)
      saturation = CapSat(saturation + sat * (255 - saturation) * (saturation / 255.0));
    else
      saturation = CapSat(saturation + sat * saturation);
  

    double C = (1-absolu(2*luminance-1))*saturation;
    double X = C * (1-absolu(fmod((hue/60),2) - 1));
    double M = luminance - C/2;

    if (hue >= 0 && hue < 60)
    {
        nrouge = C;
        nvert = X;
        nbleu = 0;
    }

    if (hue >= 60 && hue < 120)
    {
        nrouge = X;
        nvert = C;
        nbleu = 0;
    }

    if (hue >= 120 && hue < 180)
    {
        nrouge = 0;
        nvert = C;
        nbleu = X;
    }

    if (hue >= 180 && hue < 240)
    {
        nrouge = 0;
        nvert = X;
        nbleu = C;
    }

    if (hue >= 240 && hue < 300)
    {
        nrouge = X;
        nvert = 0;
        nbleu = C;
    }

    if (hue >= 300 && hue < 360)
    {
        nrouge = C;
        nvert = 0;
        nbleu = X;
    }
    if(x == 150 && y == 150)

    Bitmap->pixels[x][y].red = Cap((nrouge+M) * 255);
    Bitmap->pixels[x][y].green = Cap((nvert+M) *  255);
    Bitmap->pixels[x][y].blue = Cap((nbleu+M) * 255);
}
double max(double x, double y)
{
    double res = x < y ? y : x;
    return res;
}

double min(double x, double y)
{
    double res = x < y ? x : y;
    return res;
}

double Hue(double red, double green, double blue, double cmax, double delta)
{
  
  if(red == cmax)
    {
      double res = 60 * (fmod(((green - blue)/delta),6));
      return  res < 0 ? res + 360 : res ;
    }
    if(green == cmax) return 60 * (((blue - red)/delta) + 2);
    return 60 * (((red - green)/delta) + 4);
}

double absolu(double x)
{
    double res = x < 0 ? -x : x;
    return res;

}

int Cap(double x)
{
  if(x<0)return 0;
  if(x>255)return 0;
  return (int)x;
}

double CapSat(double x)
{
  if(x>1)return 1.0;
  if(x<-1)return -1.0;
  return x;
}
