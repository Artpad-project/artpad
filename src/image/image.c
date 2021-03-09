/*!
 *  File created on 3/5/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  File containing all the functions necessary to interact with an image.
 *  
 *  Added:
 *  3/9/2021 - image loading
 */

#include "image.h"

static void save_image_pixels(struct Image *im);
static void set_pixel(guchar *pixels, int rowstride, const struct Pixel px, const int x, const int y);

/*!
 * Load an image from path, and stores it into a structure. 
 * Save a copy of its pixels intto a Pixel array.
 * 
 * @param path relative path to the image to load
 */
struct Image *
load_image(const char *path) {
    GtkImage *im;
    GdkPixbuf *pb;
    GError *err = NULL;
    int width, height;

    struct Image *image = malloc(sizeof(struct Image));

    if (path == NULL)
        return NULL;

    pb = gdk_pixbuf_new_from_file(path, &err);

    if (pb == NULL)
        errx(err->code, "ERROR: image.c: couldn't load pixbuf (%s)", err->message);

    width = gdk_pixbuf_get_width(pb);
    height = gdk_pixbuf_get_height(pb);

    *image = (struct Image) {path, width, height, pb, NULL};

    save_image_pixels(image);
    print_pixel(image);

    return image;
}

static void
save_image_pixels(struct Image *im) {
    int row_len, bpp;
    guint len;

    guchar *pb_pixels = gdk_pixbuf_get_pixels_with_length(im->pb, &len);
    row_len = gdk_pixbuf_get_rowstride(im->pb);

    // Alloacte memory for image's pixel data : height * width * sizeof(pixel)
    // pixel(x,y) = array[x][y]
    struct Pixel **im_pixels = (struct Pixel **)malloc(im->width * sizeof(struct Pixel *));
    for (int i = 0; i < im->width; i++)
        im_pixels[i] = (struct Pixel *)malloc(im->height * sizeof(struct Pixel));

    // FIXME: RGBA
    if(gdk_pixbuf_get_bits_per_sample(im->pb)!=8)
        return;

    bpp = 3;
    for (int i = 0; i < im->height; i++) {
        for (int j = 0; j < row_len; j += bpp) {
           im_pixels[j/bpp][i].red = pb_pixels[i*row_len+j]; 
           im_pixels[j/bpp][i].green = pb_pixels[i*row_len+j+1]; 
           im_pixels[j/bpp][i].blue = pb_pixels[i*row_len+j+2]; 
        }
    }

    im->pixels = im_pixels;
}

/*!
 * Proper way to free an image structure to avoid ay memory leak. 
 * Must call after you're done processing an image.
 * 
 * @param image the image to free
 */
void
free_image(struct Image *image) {
    for (int x = 0; x < image->width; ++x)
        free(image->pixels[x]);
    free(image->pixels);
    free(image);
}


void
save_image(struct Image *im) {
    guchar *pixels = gdk_pixbuf_get_pixels(im->pb);
    int rowstride = gdk_pixbuf_get_rowstride(im->pb);
    for (int x = 0; x < im->width; ++x) {
        for (int y = 0; y < im->height; ++y) {
            struct Pixel px = im->pixels[x][y];
            set_pixel(pixels, rowstride, px, x, y);
        }
    }

    // TODO: save image to its original path
}

static void
set_pixel(guchar *pixels, int rowstride, const struct Pixel px, const int x, const int y) {
    int bpp = 3;
    pixels[y * rowstride + x * bpp + 0] = px.red;
    pixels[y * rowstride + x * bpp + 1] = px.green;
    pixels[y * rowstride + x * bpp + 2] = px.blue;
}