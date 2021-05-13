#ifndef ARTPAD_DETECTIONCOIN_H
#define ARTPAD_DETECTIONCOIN_H

#include "../image/image.h"

struct Coord
{
  int x;
  int y;
};

typedef struct Coord Coord;


Coord* Detection(Image* BitMap,double coeffgauss1, double coeffgauss2, double harriscoef, int treshold, int* nbPoint);

#endif //ARTPAD_DETECTIONCOIN_H

