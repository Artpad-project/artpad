/*!
 *  File created on 26/03/2021 (MM/DD/YYYY) by tom.aubert
 *  Contributors : tom aubert
 *
 *  File containing all the functions necessary to interact with an image.
 *  
 *  Added:
 *  3/9/2021 - image rescaling
 */

#include "../../include/image_scaling.h"


struct Image*
rescale_image(struct Image *im, int percentscale){
    int newwidth = im->width * percentscale/100;
    int newheight = im->height * percentscale/100;
    GdkPixbuf * newbuf;
    if(!(newbuf = gdk_pixbuf_scale_simple(im->pb,newwidth,newheight,GDK_INTERP_BILINEAR)))
            err(0,"ERROR: image_scaling.c - rescale_image : couldn't rescale the pixbuf");
    return load_image_from_pixbuf(newbuf);
}
