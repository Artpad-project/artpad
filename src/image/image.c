/*!
 *  File created on 3/5/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin , tom aubert
 *
 *  File containing all the functions necessary to interact with an image.
 *  
 *  Added:
 *  3/9/2021 - image loading
 */
#define _GNU_SOURCE
#include "../../include/image.h"
#include "../../include/utils.h"

#include <stdlib.h>
#include <stdio.h>
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
    for (int i = 0; i < width; i++) {
        im_pixels[i] = (struct Pixel *)malloc(height * sizeof(struct Pixel));
        memset(im_pixels[i],0, height*sizeof(struct Pixel));
    }
    
    char *file_type ;
    int val = asprintf(&file_type,"jpeg");
    if (val <0){
    	errx(1,"error while giving png");
    }
    pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB,1,8,width,height);
    *image = (struct Image){NULL, file_type, width, height, pb, im_pixels};
    return image;
}

/*!
 * realloc a matrix
 * 
 * @param origin : Old = original matrix
 * @param nRows : nomber of rows for the new matrix
 * @param nCols : nomber of columns for the new matrix
 * 
 */
struct Pixel** realloc_image(Image *im, int nRows, int nCols)
{
    if (im->width == nCols && im->height == nRows)
        return im->pixels;

    Pixel **new_pixels = malloc(nCols * sizeof(Pixel*));
    for (int i = 0; i < nCols; ++i) {
        new_pixels[i] = malloc(nRows * sizeof(Pixel));
	g_print("%d",i);
	if (i < im->width)
        	free(im->pixels[i]);
    }
    
    free(im->pixels);
    im->pixels = new_pixels;
    im->width = nCols;
    im->height = nRows;

    return new_pixels;
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
    *image = (struct Image) {NULL, NULL, width, height, pb, NULL};
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
    if(image){
    	for (int x = 0; x < image->width; ++x)
          free(image->pixels[x]);
    	free(image->pixels);
    	if (image->file)
		      free(image->file);
    	if (image->file_type)
		      free(image->file_type);
    	//free(image);
    }
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

/*!
 * actualise the pixbuff of an image.
 * 
 * @param im the image to sav 
 * */
void
actualise_image(struct Image *im, int xinit, int yinit, int width,int heigth) {
    guchar *pixels = gdk_pixbuf_get_pixels(im->pb);
    int rowstride = gdk_pixbuf_get_rowstride(im->pb);
    int tot = 0;
    for (int x = xinit; x < width; ++x) {
        for (int y = yinit; y < heigth; ++y) {
            struct Pixel px = im->pixels[x][y];
            set_pixel(pixels, rowstride, px, x, y);
	    tot++;
        }
    }
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

struct Image *create_copy_image(Image *im);

/*!
 * copy an image, and put it into another image. 

 * 
 * @param src : original image
 * @param dst : copy image
 * 
 */

struct Image *copy_image(Image *src, Image *dst){
    if (!dst)
        return create_copy_image(src);

    if (src->width != dst->width || src->height != dst->height)
        realloc_image(dst,src->height,src->width);
  
    dst->file = src->file;
    dst->file_type = src->file_type;
    dst->pb = src->pb;

    if (!dst->pixels) {
        dst->pixels = malloc(dst->width * sizeof(Pixel*));
        for (int i = 0; i < dst->width; ++i)
            dst->pixels[i] = malloc(dst->height * sizeof(Pixel));
    }
  
    for(int i = 0;i<src->width;i++)
        for(int j = 0;j<src->height;j++){
            dst->pixels[i][j].red = src->pixels[i][j].red;
            dst->pixels[i][j].blue = src->pixels[i][j].blue;
            dst->pixels[i][j].green = src->pixels[i][j].green;
            dst->pixels[i][j].alpha = src->pixels[i][j].alpha;
        }

    return dst;
}


/*!
 * Copy an image
 * 
 * @return A copy of the image
 */
struct Image *
create_copy_image(Image *im) {
    if (!im)
        return NULL;

    struct Image *new_image = malloc(sizeof(struct Image));
    *new_image = (struct Image) {
        NULL,
        NULL,
        im->width, im->height,
        NULL,
        NULL
    };

    if (im->file)
      new_image->file = strdup(im->file);
    if (im->file_type)
      new_image->file_type = strdup(im->file_type);

    new_image->pb = im->pb;
    new_image->pixels = malloc(new_image->width * sizeof(Pixel*));
    for (int i = 0; i < new_image->width; ++i) {
        new_image->pixels[i] = malloc(new_image->height * sizeof(Pixel));
        memcpy(new_image->pixels[i], im->pixels[i], sizeof(Pixel) * new_image->height);
    }

    return new_image;
}


struct Pixel 
pixel_from_GdkRGBA(GdkRGBA *col){
	struct Pixel pixel;
	pixel.red = col->red*255;
	pixel.green = col->green*255;
	pixel.blue = col->blue*255;
	pixel.alpha = col->alpha*255;
	return pixel;
}
