//
// Created by lecom_qxrxxt0 on 10/03/2021.
//

#ifndef ARTPAD_PUISSANCEN_H
#define ARTPAD_PUISSANCEN_H

struct Pixel{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Image{
    int width, height;
    struct Pixel** pixels;
};

typedef struct Pixel Pixel;
typedef struct Image Image;

void BalanceAbsolue(Image* Bitmap, double coeflum);

#endif //ARTPAD_PUISSANCEN_H
