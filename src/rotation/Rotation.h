#ifndef ARTPAD_ROTATION_H
#define ARTPAD_ROTATION_H

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

#endif //ARTPAD_ROTATION_H
