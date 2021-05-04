#include "BalanceAuto.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

double max(double x, double y);
double min(double x, double y);
double Hue(double red, double green, double blue, double cmax, double delta);
double absolu(double x);

void BalanceAuto(Image* Bitmap)
{

    int nbLine = Bitmap->height;
    int nbCol = Bitmap->width;

    int nbPix = nbLine * nbCol;

    int* HistoLuminance = malloc(101 * sizeof(double));
    memset(HistoLuminance, 0, 101);

    int* cdf = malloc(101 * sizeof(double));
    memset(cdf, 0, 101);

    int* equalized = malloc(101 * sizeof(double));
    memset(equalized, 0, 101);

    for (int i = 0; i < nbLine; ++i) {

        for (int j = 0; j < nbCol; ++j) {

            double nrouge = Bitmap->pixels[i][j].red / 255.0;
            double nvert = Bitmap->pixels[i][j].green / 255.0;
            double nbleu = Bitmap->pixels[i][j].blue / 255.0;

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

            double nrouge = Bitmap->pixels[i][j].red / 255.0;
            double nvert = Bitmap->pixels[i][j].green / 255.0;
            double nbleu = Bitmap->pixels[i][j].blue / 255.0;

            double cmax = max(nbleu, max(nrouge,nvert));
            double cmin = min(nbleu, min(nvert,nrouge));

            double delta = cmax - cmin;

            double hue = delta == 0 ? 0 : Hue(nrouge,nvert,nbleu,cmax,delta) ;
            double luminance = ((cmax + cmin)/2.0)*100;
            double saturation = delta == 0 ? 0 : delta /(1-absolu(2*luminance-1)) ;

            double newluminance =  (round((cdf[(int)luminance] - cdftmin)/(double)((nbPix-cdftmin)*255.0)))/100.0;

            double C = (1-absolu(2*newluminance-1))*saturation;
            double X = C * (1-absolu((int)(hue/60)%2 - 1));
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

            Bitmap->pixels[i][j].red = (int)((nrouge+M) * 255);
            Bitmap->pixels[i][j].green = (int)((nvert+M) *  255);
            Bitmap->pixels[i][j].blue = (int)((nbleu+M) * 255);

        }
    }

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
    if(red == cmax) return  60 * ((int)((green - blue)/delta) %6);
    if(green == cmax) return 60 * (((blue - red)/delta) + 2);
    return 60 * (((red - green)/delta) + 4);
}

double absolu(double x)
{
    double res = x < 0 ? -x : x;
    return res;

}