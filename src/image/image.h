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

// FIXME: RGBA for more control
struct Pixel {
    // 0 to 255 : 8bits
    unsigned char red;
    unsigned char green;
    unsigned char blue;
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

struct Image *load_image(char *path);
void save_image(struct Image *im, char *out, char *ftype);
void free_image(struct Image *im);

#endif // IMAGE_H_
