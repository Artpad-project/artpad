/*!
 *  File created on 3/5/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin , tom aubert
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
static void set_pixel(guchar *pixels, int rowstride, struct Pixel px, int x, int y);



/*
 * create a new image of size (width*height), and stores it into a structure. 
 *  pixels intto the Pixel array are white.
 * 
 * @param path relative path to the image to load
 */

struct Image *
new_image(int width,int height) {
    GdkPixbuf *pb;
    
    struct Image *image = malloc(sizeof(struct Image)); 

    struct Pixel **im_pixels = (struct Pixel **)malloc(width * sizeof(struct Pixel *));
    for (int i = 0; i < width; i++)
        im_pixels[i] = (struct Pixel *)malloc(height * sizeof(struct Pixel));
    
    for(int x=0 ;x<width;x++){
        for(int y = 0;y<height;y++){
            im_pixels[x][y].alpha = 255;
            im_pixels[x][y].red = 255;
            im_pixels[x][y].green = 255;
            im_pixels[x][y].blue = 255;
        }
    }
    
    pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB,1,8,width,height); 
    *image = (struct Image){NULL, "jpg", width, height, pb, im_pixels};
    return image;
}

/*!
 * realloc a matric
 * 
 * @param origin : Old = original matrix
 * @param nRows : nomber of rows for the new matrix
 * @param nCols : nomber of columns for the new matrix
 * 
 */
struct Pixel** reallocArray (Pixel **Old, int nRows, int nCols)
{
    // use a single realloc for the char pointers to the first char of each row
    // so we reallocate space for the pointers and then space for the actual rows.
    Pixel **pArray = realloc (Old, sizeof(Pixel *) * nRows + sizeof(Pixel) * nCols * nRows);

    if (pArray) {
        // calculate offset to the beginning of the actual data space
        Pixel *pOffset = (Pixel *)(pArray + nRows);

        // fix up the pointers to the individual rows
        for (int i = 0; i < nRows; i++) {
            pArray[i] = pOffset;
            pOffset += nCols;
        }
    }
    return pArray;
}

/*!
 * copy an image, and put it into another image. 
 * Save a copy of the original matrix of pixel
 * 
 * @param origin : original image
 * @param copy : copy image
 * 
 */
void copy_image(Image *origin, Image *copy){
     
    copy->pixels = reallocArray(copy->pixels,origin->height,origin->width);
    copy->file = origin->file;
    copy->file_type = origin->file_type;
    copy->width = origin->width;
    copy->height = origin->height;
    copy->pb = origin->pb;


    for(int i = 0;i<origin->height;i++)
        for(int j = 0;j<origin->width;j++){
            copy->pixels[i][j].red = origin->pixels[i][j].red;
            copy->pixels[i][j].blue = origin->pixels[i][j].blue;
            copy->pixels[i][j].green = origin->pixels[i][j].green;
            copy->pixels[i][j].alpha = origin->pixels[i][j].alpha;
        }
}



/*!
 * copy an image, and stores it into a structure. 
 * Save a copy of the original matrix of pixel
 * 
 * @param origin : original image
 * 
 */

struct Image*
create_copy_image(Image *origin){
    struct Pixel **im_pixels = (struct Pixel **)malloc(origin->width * sizeof(struct Pixel *));
    for (int i = 0; i < origin->width; i++)
        im_pixels[i] = (struct Pixel *)malloc(origin->height * sizeof(struct Pixel));
    
    for(int i = 0;i<origin->height;i++)
        for(int j = 0;j<origin->width;j++){
            im_pixels[i][j].red = origin->pixels[i][j].red;
            im_pixels[i][j].blue = origin->pixels[i][j].blue;
            im_pixels[i][j].green = origin->pixels[i][j].green;
            im_pixels[i][j].alpha = origin->pixels[i][j].alpha;
        }
    struct Image *new_image = malloc(sizeof(struct Image));
    *new_image = (struct Image){origin->file,origin->file_type,origin->width,origin->height,origin->pb,im_pixels};
    return new_image;
}

/*!
 * Load an image from pixbuff, and stores it into a structure. 
 * Save a copy of its pixels intto a Pixel array.
 * 
 * @param pb relative pixbuff to the image to load
 */

struct Image*
load_image_from_pixbuf(GdkPixbuf *pb){
    if(!pb)
        err(0,"ERROR: image.c - load_image_from_pixbuff : pb == NULL");
    
    struct Image *image = malloc(sizeof(struct Image));
    int width = gdk_pixbuf_get_width(pb);
    int height = gdk_pixbuf_get_height(pb);
    *image = (struct Image) {NULL, "jpg", width, height, pb, NULL};
    save_image_pixels(image);
    return image;


}


/*!
 * Load an image from path, and stores it into a structure. 
 * Save a copy of its pixels intto a Pixel array.
 * 
 * @param path relative path to the image to load
 */
struct Image *
load_image(char *path) {
    GdkPixbuf *pb;
    int width, height;
    GError *err = NULL;

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
    *image = (struct Image) {strdup(path), file_type, width, height, pb, NULL};

    save_image_pixels(image);

    return image;
}

static char *
parse_image_path(char *path) {
    char *filetype = strpbrk(path, ".") + 1;

    if (!strcmp(filetype, "jpg"))
        filetype = "jpeg";

    return strdup(filetype);
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
    free(image->file);
    free(image->file_type);
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

/*!
 * Copy an image
 * 
 * @return A copy of the image
 */
struct Image *
copy_image(const struct Image *im) {
    struct Image *new_image = malloc(sizeof(struct Image));

    *new_image = (struct Image) {
        strdup(im->file),
        strdup(im->file_type),
        im->width, im->height,
        NULL,
        NULL
    };

    new_image->pb = gdk_pixbuf_copy(im->pb);
    save_image_pixels(new_image);

    return new_image;
}
