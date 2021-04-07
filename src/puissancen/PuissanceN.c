
#include "PuissanceN.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

double Encadrement(double inconnue,const double* HistoColor);
double Exposant(double* HistoColor, double CoefCible, double precision);
void TableauCanal(double* HistoColor, int Index, double Exposant);

void BalanceAbsolue(Image* Bitmap, double coeflum)
{
    printf("CB_value In Balace_absolue : %f\n",coeflum);
    printf("Pixel values: %i,%i,%i\n ",Bitmap->pixels[10][10].red,Bitmap->pixels[10][10].green,Bitmap->pixels[10][10].blue);
    /* ------------------------ Initialisation ----------------------------- */

    int nbLine = Bitmap->height;
    int nbCol = Bitmap->width ;

    int nbPix = nbLine * nbCol;

    double amplification = 0.8;

    double* HistoRed = malloc(256 * sizeof(double));
    memset(HistoRed, 0, 256);

    double* HistoVert = malloc(256 * sizeof(double));
    memset(HistoVert, 0, 256);

    double* HistoBleu = malloc(256 * sizeof(double));
    memset(HistoBleu, 0, 256);

    /* -------------------------- Histogramme ----------------------------- */

    int maxRouge = 0;
    int minRouge = 255;

    int maxVert = 0;
    int minVert = 255;

    int maxBleu = 0;
    int minBleu = 255;

    int Total = 0;

    for (int y = 0; y < nbLine ; ++y)
    {
        for (int x = 0; x < nbCol; ++x)
        {
            Pixel actual = Bitmap->pixels[x][y];
            int ValueRed = actual.red;
            int ValueGreen = actual.green;
            int ValueBlue = actual.blue;

            HistoRed[ValueRed] += (1.0/nbPix);
            HistoVert[ValueGreen] += (1.0/nbPix);
            HistoBleu[ValueBlue] += (1.0/nbPix);

            Total += ValueBlue + ValueGreen + ValueRed;

            if(ValueRed > maxRouge) maxRouge = ValueRed;
            if(ValueRed < minRouge) minRouge = ValueRed;

            if(ValueBlue > maxBleu) maxBleu = ValueBlue;
            if(ValueBlue < minBleu) minBleu = ValueBlue;

            if(ValueGreen > maxVert) maxVert = ValueGreen;
            if(ValueGreen < minVert) minVert = ValueGreen;
        }
    }

    double IntenMoy = Total / (3.0*nbPix);
    double CoefMoyen = IntenMoy / 255;

    /* --------------------- Exposant Transformation ----------------------- */

    if (maxVert == minVert || maxBleu == minBleu || maxRouge == minRouge) return;

    double Red1 = 1.0 * 255 / (maxRouge -  minRouge);
    double Red0 = - Red1 * minRouge;

    double Green1 = 1.0 * 255 / (maxVert - minVert);
    double Green0 = - Green1 * minVert;

    double Blue1 = 1.0 * 255 / (maxBleu - minBleu);
    double Blue0 = - Blue1 * minBleu;

    double ExposantRed;
    double ExposantGreen;
    double ExposantBlue;

    double Prec = 0.4;

    double CoefCible;

    if(coeflum > 0) CoefCible = CoefMoyen + coeflum * amplification * (1 - CoefMoyen);
    else CoefCible = CoefMoyen + coeflum * amplification * CoefMoyen;

    /* ------------------------ Canal Chromatique -------------------------- */

     ExposantRed = Exposant(HistoRed, CoefCible, Prec);
     ExposantGreen = Exposant(HistoVert, CoefCible, Prec);
     ExposantBlue = Exposant(HistoBleu, CoefCible, Prec);

    /* -------------------------- Transformation --------------------------- */

    double* TableauRed = malloc(256 * sizeof(double));
    memset(TableauRed,0,256);
    
    double* TableauGreen = malloc(256 * sizeof(double));
    memset(TableauGreen,0,256);
    
    double* TableauBlue = malloc(256 * sizeof(double));
    memset(TableauBlue,0,256);

    for (int i = 0; i < 256; ++i)
    {
        TableauCanal(TableauRed, i, ExposantRed);
        TableauCanal(TableauGreen, i, ExposantGreen);
        TableauCanal(TableauBlue, i, ExposantBlue);
    }

    /* ---------------------------- Application ---------------------------- */

    for (int y = 0; y < nbLine ; ++y)
    {
        for (int x = 0; x < nbCol; ++x)
        {
            Pixel actual = Bitmap->pixels[x][y];
            int ValueRed = actual.red;
            int ValueGreen = actual.green;
            int ValueBlue = actual.blue;

            int IndexRouge = (int)(round(Red1 * ValueRed + Red0));
            IndexRouge = IndexRouge > 255 ? 255 : IndexRouge;
            IndexRouge = IndexRouge < 0  ? 0 : IndexRouge;

            int IndexVert = (int)(round(Green1 * ValueGreen + Green0));
            IndexVert = IndexVert > 255 ? 255 : IndexVert;
            IndexVert = IndexVert < 0  ? 0 : IndexVert;

            int IndexBleu = (int)(round(Blue1 * ValueBlue + Blue0));
            IndexBleu = IndexBleu > 255 ? 255 : IndexBleu;
            IndexBleu = IndexBleu < 0  ? 0 : IndexBleu;

            Bitmap->pixels[x][y].red = (unsigned char)TableauRed[IndexRouge];
            Bitmap->pixels[x][y].green = (unsigned char)TableauGreen[IndexVert];
            Bitmap->pixels[x][y].blue = (unsigned char)TableauBlue[IndexBleu];
	   
        }
    }
    printf("Print values: %f,%f,%f,%f\n ",ExposantRed, ExposantGreen, ExposantBlue,HistoRed[250]);
    free(HistoRed);
    free(HistoVert);
    free(HistoBleu);
    free(TableauRed);
    free(TableauGreen);
    free(TableauBlue);
   
}


double Encadrement(double inconnue, const double* HistoColor)
{
    double search = 0.0;
    for (int i = 0; i < 256; ++i) {
        search += HistoColor[i]*pow((1.0*i/255),inconnue);
    }
    return search;
}

double Exposant(double* HistoColor, double CoefCible, double precision)
{
    double xinf = 0.0;
    double xsup = 0.0;
    double x = 1.0;
    int flag = 0;

    while(!flag)
    {
        if(Encadrement(x, HistoColor) > CoefCible)
        {
            xinf = x;
            xsup = x;
            x = 2.0 * x;
        }
        else
        {
            xsup = x;
            x = 0.5 * (xinf + xsup);
            flag = 1;
        }
    }

    int imprecis = 1;

    while (imprecis)
    {
      if(Encadrement(x,HistoColor) > CoefCible) xinf = x;
      else xsup = x;
      x = 0.5 * (xinf + xsup);
      if((xsup - xinf) < precision) imprecis = 0;
    }

    return x;
}

void TableauCanal(double* HistoColor, int Index, double Exposant)
{
  int Value = (int)(round(255 * pow((1.0 * Index/255 ),Exposant)));
    Value = Value > 255 ? 255 : Value;
    Value = Value < 0 ? 0 : Value;
    HistoColor[Index] = Value;
}
