/*!
 *  File created on 3/5/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  File containing all the necessary functions prototypes to interact with an image.
 *  It is the core of this project.
 *
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h>


// image.c

#define DIF_MAX(a,b) (a < b) ? b-a : a-b
#define DIF_PX(_px1, _px2) (Pixel) {    \
        DIF_MAX(_px1.red, _px2.red),    \
        DIF_MAX(_px1.green, _px2.green),\
        DIF_MAX(_px1.blue, _px2.blue),  \
        _px1.alpha                      \
    }

#define SUM_PX(_px) (_px.red + _px.green + _px.blue)

#define MULT_PX(_px, factor) (Pixel) { \
    factor * _px.red,                  \
    factor * _px.green,                \
    factor * _px.blue,                 \
    _px.alpha                          \
}

#define ADD_PX(_px1, _px2) (Pixel) { \
    _px1.red + _px2.red,             \
    _px1.green + _px2.green,         \
    _px2.blue + _px2.blue,           \
    _px1.alpha                       \
}

#define EQ_PX(_px1, _px2) dp {_px1.red == _px2.red && _px1.blue == _px2.blue && _px1.green == _px2.green)

#define BLACK (Pixel){0, 0, 0, 255};
#define WHITE (Pixel){255, 255, 255, 255};
#define EMPTY (Pixel){0, 0, 0, 0};

struct Pixel {
    // 0 to 255 : 8bits
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

struct Image {
    char *file;
    char *file_type;
    int width, height;
    GdkPixbuf *pb;
    struct Pixel **pixels;
};

typedef struct Image Image;
typedef struct Pixel Pixel;

struct Image *new_image(int width,int height);
struct Image *copy_image(Image *src, Image *dst);
struct Image *create_copy_image(Image *src);


struct Image *load_image_from_pixbuf(GdkPixbuf *pb);
struct Image *load_image(char *path);

void save_image(struct Image *im, char *out, char *ftype);
void actualise_image (struct Image *im,int xinit,int yinit,int width,int height);
void free_image(struct Image *im);

// mask.c

struct ImageMask {
    int width, height;
    uint8_t **area;
    Image *image;
    Image *mask;
};

typedef struct ImageMask ImageMask;

ImageMask init_mask(Image *im);
Pixel edit_mask(ImageMask mask, int x, int y, int value);
void free_mask(ImageMask mask);

struct Pixel  pixel_from_GdkRGBA(GdkRGBA *col); 
#endif // IMAGE_H_
