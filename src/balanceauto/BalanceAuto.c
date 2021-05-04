#include "BalanceAuto.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

double max(double x, double y);
double min(double x, double y);
double Hue(double red, double green, double blue, double cmax, double delta);
double absolu(double x);
int Cap(double x);
double CapSat(double x);

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

            double cmax = max(nbleu, max(nrouge,nvert));
            double cmin = min(nbleu, min(nvert,nrouge));

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

            double cmax = max(nbleu, max(nrouge,nvert));
            double cmin = min(nbleu, min(nvert,nrouge));

            double delta = cmax - cmin;

            double hue = delta == 0 ? 0 : Hue(nrouge,nvert,nbleu,cmax,delta) ;
            double luminance = ((cmax + cmin)/2.0)*100;
            double saturation = delta == 0 ? 0 : CapSat(delta /(1-absolu(2*luminance/100-1))) ;

            double newluminance =  round( ((cdf[(int)luminance] - cdftmin) / (double)( (nbPix-cdftmin))) * 100.0 ) /100.0;

            double C = (1-absolu(2*newluminance-1))*saturation;
            double X = C * (1-absolu(fmod((hue/60),2) - 1));
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
	    //printf("%f %f %f \n",nrouge,nvert,nbleu);
            Bitmap->pixels[j][i].red = Cap((nrouge+M) * 255);
            Bitmap->pixels[j][i].green = Cap((nvert+M) *  255);
            Bitmap->pixels[j][i].blue = Cap((nbleu+M) * 255);

        }
    }

    free(HistoLuminance);
    free(cdf);

}

double max(double x, double y)
{
    double res = x < y ? y : x;
    return res;
}

double min(double x, double y) {
    double res = x < y ? x : y;
    return res;
}

double Hue(double red, double green, double blue, double cmax, double delta)
{
  if(red == cmax)
    {
      double res =  60 * (fmod(((green - blue)/delta),6));
      return res <0 ? res +360 : res;
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
  double res = x < 0 ? 0 : x;
  res = x > 255 ? 255 : x;
  return (int)res;
}

double CapSat(double x)
{
  if(x>1)return 1.0;
  if(x<-1)return -1.0;
  return x;

}
