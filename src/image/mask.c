
/*!
 *  File created on 3/10/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  Source code for all functions related to image masks
 *  
 *  Added:
 *  3/10/2021 - init_area, edit_area, free_area
 *  3/12/2021 - renamed ImageArea to ImageMask
 */

#include "image.h"

/*!
 * Create an empty area of the size of an image
 * 
 * @param im The base image
 * @return An empty area
 */
ImageMask 
init_mask(Image *im) {
    uint8_t **area;
    Image *mask;
    ImageMask im_mask;
    
    area = malloc(im->width * sizeof(uint8_t*));
    mask = copy_image(im);
    im_mask = (ImageMask){im->width, im->height, area, im, mask};

    // Init an empty area and an empty mask (transparent black)
    for (int i = 0; i < im_mask.width; i++) {
        area[i] = malloc(im_mask.height * sizeof(uint8_t));
        memset(area[i], FALSE, im_mask.height);
        for (int y = 0; y < im_mask.height; ++y)
            mask->pixels[i][y] = EMPTY;
    }

    return im_mask;  
}

/*!
 * Edit a pixel in the area and set the mask accordingly
 * 
 * @param area The area to edit
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @param value FALSE if we want to remove the pixel from the area, TRUE otherwise
 * @return The new value of the pixel in the mask
 */
Pixel
edit_mask(ImageMask mask, int x, int y, int value) {
    mask.area[x][y] = value;
    mask.mask->pixels[x][y] = value ? mask.image->pixels[x][y] : EMPTY;

    return mask.mask->pixels[x][y];
}

/*!
 * Proper way to free the content of an area
 * 
 * @param area The area to free
 */
void
free_mask(ImageMask mask) {
    free_image(mask.mask);
    for (int i = 0; i < mask.width; ++i)
        free(mask.area[i]);
    free(mask.area);
}