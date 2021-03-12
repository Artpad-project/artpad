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
#include <string.h>

static char * parse_image_path(char *path);
static void save_image_pixels(struct Image *im);
static void set_pixel(guchar *pixels, int rowstride, const struct Pixel px, const int x, const int y);

/*!
 * Load an image from path, and stores it into a structure. 
 * Save a copy of its pixels intto a Pixel array.
 * 
 * @param path relative path to the image to load
 */
struct Image *
load_image(char *path) {
    GdkPixbuf *pb;
    GError *err = NULL;
    int width, height;
    char *file_type;

    struct Image *image = malloc(sizeof(struct Image));

    if (path == NULL)
        return NULL;

    pb = gdk_pixbuf_new_from_file(path, &err);

    if (pb == NULL)
        errx(err->code, "ERROR: image.c: couldn't load pixbuf (%s)", err->message);

    width = gdk_pixbuf_get_width(pb);
    height = gdk_pixbuf_get_height(pb);

    file_type = parse_image_path(path);
    *image = (struct Image) {path, file_type, width, height, pb, NULL};

    save_image_pixels(image);

    return image;
}

static char *
parse_image_path(char *path) {
    char *filetype = strpbrk(path, ".") + 1;

    if (!strcmp(filetype, "jpg"))
        filetype = "jpeg";

    return filetype;
}

static void
save_image_pixels(struct Image *im) {
    int rowstride, n_channels;
    guchar *pb_pixels, *p;
    
    // Alloacte memory for image's pixel data : height * width * sizeof(pixel)
    // pixel(x,y) = array[x][y]
    struct Pixel **im_pixels = (struct Pixel **)malloc(im->width * sizeof(struct Pixel *));
    for (int i = 0; i < im->width; i++)
        im_pixels[i] = (struct Pixel *)malloc(im->height * sizeof(struct Pixel));

    // Set image to RGBA and get its size
    im->pb = gdk_pixbuf_add_alpha(im->pb, FALSE, 0, 0, 0);
    pb_pixels = gdk_pixbuf_get_pixels(im->pb);
    n_channels = gdk_pixbuf_get_n_channels (im->pb);
    rowstride = gdk_pixbuf_get_rowstride (im->pb);
    
    // Check that image is of the correct type
    g_assert (gdk_pixbuf_get_colorspace (im->pb) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (im->pb) == 8);
    g_assert (n_channels == 4);

    for (int x = 0; x < im->width; ++x) {
        for (int y = 0; y < im->height; ++y) {
            p = pb_pixels + y * rowstride + x * n_channels;
            im_pixels[x][y].red = p[0];
            im_pixels[x][y].green = p[1];
            im_pixels[x][y].blue = p[2];
            im_pixels[x][y].alpha = p[3];
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

/*!
 * Save image into a file. 
 * 
 * @param im the image to save
 * @param out the file to save into. If NULL the image's own file will be edited.
 * @param ftype the file format. If NULL the image's own file format will be used instead.
 */
void
save_image(struct Image *im, char *out, char *ftype) {
    guchar *pixels = gdk_pixbuf_get_pixels(im->pb);

    if (!out)
        out = im->file;
    if (!ftype)
        ftype = im->file_type;

    int rowstride = gdk_pixbuf_get_rowstride(im->pb);
    for (int x = 0; x < im->width; ++x) {
        for (int y = 0; y < im->height; ++y) {
            struct Pixel px = im->pixels[x][y];
            set_pixel(pixels, rowstride, px, x, y);
        }
    }

    if (!gdk_pixbuf_save(im->pb, out, ftype, NULL, NULL))
        errx(1, "Error: couldn't save image (%s - > %s)\n", im->file, out);
}

static void
set_pixel(guchar *pixels, int rowstride, const struct Pixel px, const int x, const int y) {
    int n_channels = 4;
    pixels += y * rowstride + x * n_channels;
    pixels[0] = px.red;
    pixels[1] = px.green;
    pixels[2] = px.blue;
    pixels[3] = px.alpha;
}
