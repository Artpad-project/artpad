
/*!
 *  File created on 3/10/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  Source code for all area related functions.
 *  
 *  Added:
 *  3/10/2021 - init_area, edit_area, free_area
 */

#include "image.h"

/*!
 * Create an empty area of the size of an image
 * 
 * @param im The base image
 * @return An empty area
 */
ImageArea 
init_area(Image *im) {
    uint8_t **area;
    Image *mask;
    ImageArea im_area;
    
    area = malloc(im->width * sizeof(uint8_t*));
    mask = copy_image(im);
    im_area = (ImageArea){im->width, im->height, area, im, mask};

    // Init an empty area and an empty mask (transparent black)
    for (int i = 0; i < im_area.width; i++) {
        area[i] = malloc(im_area.height * sizeof(uint8_t));
        memset(area[i], FALSE, im_area.height);
        for (int y = 0; y < im_area.height; ++y)
            mask->pixels[i][y] = EMPTY;
    }

    return im_area;  
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
edit_area(ImageArea area, int x, int y, int value) {
    area.area[x][y] = value;
    area.mask->pixels[x][y] = value ? area.image->pixels[x][y] : EMPTY;

    return area.mask->pixels[x][y];
}

/*!
 * Proper way to free the content of an area
 * 
 * @param area The area to free
 */
void
free_area(ImageArea area) {
    free_image(area.mask);
    for (int i = 0; i < area.width; ++i)
        free(area.area[i]);
    free(area.area);
}