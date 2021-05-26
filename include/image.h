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
struct Image *copy_image(Image *origin, Image *copy);
struct Image *create_copy_image(const struct Image * origin);

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
