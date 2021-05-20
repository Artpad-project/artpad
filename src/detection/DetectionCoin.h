#ifndef ARTPAD_DETECTIONCOIN_H
#define ARTPAD_DETECTIONCOIN_H

#include "../image/image.h"

struct Coord
{
    int x, y;
};

typedef struct Coord Coord;


Coord* Detection(Image* BitMap,double coeffgauss1, double coeffgauss2, double harriscoef, int treshold);

#endif //ARTPAD_DETECTIONCOIN_H

