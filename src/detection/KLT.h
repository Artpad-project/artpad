#ifndef ARTPAD_KLT_H
#define ARTPAD_KLT_H

struct Pixel{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Image{
    int width, height;
    struct Pixel** pixels;
};

struct Coord
        {
    int x, y;
        };

typedef struct Coord Coord;
typedef struct Pixel Pixel;
typedef struct Image Image;

#endif //ARTPAD_KLT_H
