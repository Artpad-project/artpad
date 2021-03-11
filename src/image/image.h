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

struct Image * load_image(char *path);
void save_image(const struct Image *im, char *out, char *ftype);
struct Image * copy_image(const struct Image *im);
void free_image(struct Image *im);

// area.c

struct ImageArea {
    int width, height;
    uint8_t **area;
    Image *image;
    Image *mask;
};

typedef struct ImageArea ImageArea;

ImageArea init_area(Image *im);
Pixel edit_area(ImageArea area, int x, int y, int value);
void free_area(ImageArea area);

#endif // IMAGE_H_
