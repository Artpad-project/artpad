#include "Saturation.h"

double max(double x, double y);
double min(double x, double y);
double Hue(double red, double green, double blue, double cmax, double delta);
double absolu(double x);

void Saturation(Image Bitmap, int x, int y,double sat)
{
    double nrouge = Bitmap.pixels[x][y].red / 255.0;
    double nvert = Bitmap.pixels[x][y].green / 255.0;
    double nbleu = Bitmap.pixels[x][y].blue / 255.0;

    double cmax = max(nbleu, max(nrouge,nvert));
    double cmin = min(nbleu, min(nvert,nrouge));
    double delta = cmax - cmin;

    double hue = delta == 0 ? 0 : Hue(nrouge,nvert,nbleu,cmax,delta) ;
    double luminance = (cmax + cmin)/2.0;
    double saturation = delta == 0 ? 0 : delta /(1-absolu(2*luminance-1)) ;

    saturation *= sat;

    double C = (1-absolu(2*luminance-1))*saturation;
    double X = C * (1-absolu((int)(hue/60)%2 - 1));
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

    Bitmap.pixels[x][y].red = (int)(nrouge * 255);
    Bitmap.pixels[x][y].green = (int)(nvert *  255);
    Bitmap.pixels[x][y].blue = (int)(nbleu * 255);
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
    if(red == cmax) return  60 * ((int)((green - blue)/delta) %6);
    if(green == cmax) return 60 * (((blue - red)/delta) + 2);
    return 60 * (((red - green)/delta) + 4);
}

double absolu(double x)
{
    double res = x < 0 ? -x : x;
    return res;

}