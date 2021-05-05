#include "BalanceAuto.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

double Hue2(double red, double green, double blue, double cmax, double delta);
int Cap2(double x);
double CapSat2(double x);

void BalanceAuto(Image* Bitmap)
{

    int nbLine = Bitmap->height;
    int nbCol = Bitmap->width;

    int nbPix = nbLine * nbCol;

    int* HistoLuminance = malloc(101 * sizeof(int));
    for(int i = 0; i<101; ++i) HistoLuminance[i]=0;

    int* cdf = malloc(101 * sizeof(int));
    for(int i = 0; i<101; ++i) cdf[i]=0;


    for (int i = 0; i < nbLine; ++i) {

        for (int j = 0; j < nbCol; ++j) {

            double nrouge = Bitmap->pixels[j][i].red / 255.0;
            double nvert = Bitmap->pixels[j][i].green / 255.0;
            double nbleu = Bitmap->pixels[j][i].blue / 255.0;

            double cmax = MAX(nbleu, MAX(nrouge,nvert));
            double cmin = MIN(nbleu, MIN(nvert,nrouge));

            int luminance = (int)(((cmax + cmin)/2.0)*100);

            HistoLuminance[luminance] += 1;
        }

    }

    int cdft = 0;
    int cdftmin = 0;
    for (int i = 0; i < 101; ++i) {
        cdft += HistoLuminance[i];
        cdf[i] = cdft;
        if(!cdftmin && cdft) cdftmin = cdft;
    }

    for (int i = 0; i < nbLine; ++i) {

        for (int j = 0; j < nbCol; ++j) {

            double nrouge = Bitmap->pixels[j][i].red / 255.0;
            double nvert = Bitmap->pixels[j][i].green / 255.0;
            double nbleu = Bitmap->pixels[j][i].blue / 255.0;

            double cmax = MAX(nbleu, MAX(nrouge,nvert));
            double cmin = MIN(nbleu, MIN(nvert,nrouge));

            double delta = cmax - cmin;

            double hue = delta == 0 ? 0 : Hue2(nrouge,nvert,nbleu,cmax,delta) ;
            double luminance = ((cmax + cmin)/2.0)*100;
            double saturation = delta == 0 ? 0 : CapSat2(delta /(1-ABS(2*luminance/100-1))) ;

            double newluminance =  round( ((cdf[(int)luminance] - cdftmin) / (double)( (nbPix-cdftmin))) * 100.0 ) /100.0;

            double C = (1-ABS(2*newluminance-1))*saturation;
            double X = C * (1-ABS(fmod((hue/60),2) - 1));
            double M = newluminance - C/2;

            if (hue >= 0 && hue < 60)
            {
                nrouge = C;
                nvert = X;
                nbleu = 0;
            }

            else if (hue >= 60 && hue < 120)
            {
                nrouge = X;
                nvert = C;
                nbleu = 0;
            }

            else if (hue >= 120 && hue < 180)
            {
                nrouge = 0;
                nvert = C;
                nbleu = X;
            }

            else if (hue >= 180 && hue < 240)
            {
                nrouge = 0;
                nvert = X;
                nbleu = C;
            }

            else if (hue >= 240 && hue < 300)
            {
                nrouge = X;
                nvert = 0;
                nbleu = C;
            }

            else if (hue >= 300 && hue < 360)
            {
                nrouge = C;
                nvert = 0;
                nbleu = X;
            }
	       Bitmap->pixels[j][i].red = Cap2((nrouge+M) * 255);
            Bitmap->pixels[j][i].green = Cap2((nvert+M) *  255);
            Bitmap->pixels[j][i].blue = Cap2((nbleu+M) * 255);

        }
    }

    free(HistoLuminance);
    free(cdf);

}


double Hue2(double red, double green, double blue, double cmax, double delta)
{
  if(red == cmax)
    {
      double res =  60 * (fmod(((green - blue)/delta),6));
      return res <0 ? res +360 : res;
    }
   if(green == cmax) return 60 * (((blue - red)/delta) + 2);
    return 60 * (((red - green)/delta) + 4);
}

int Cap2(double x)
{
  double res = x < 0 ? 0 : x;
  res = x > 255 ? 255 : x;
  return (int)res;
}

double CapSat2(double x)
{
  if(x>1)return 1.0;
  if(x<-1)return -1.0;
  return x;

}
