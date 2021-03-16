#ifndef ARTPAD_SATURATION_H
#define ARTPAD_SATURATION_H

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

void Saturation(Image Bitmap, int x, int y,double sat);

#endif //ARTPAD_SATURATION_H
