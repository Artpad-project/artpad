//
// Created by leo.duboin on 3/24/21.
//

#ifndef SRC_FILTER_H
#define SRC_FILTER_H

#include <math.h>
#define PI 3.1415926535

#include "image.h"
#include "../utils/utils.h"

/*!
 * Basic structure of a kernel used for convolution.
 * @param matrix the kernel itself
 * @param size the size of the kernel (size * size)
*/
typedef struct Kernel {
    int *matrix;
    int size;  // kernel : size * size
} Kernel;

/*!
 * List of all the different filter types
*/
typedef enum filter_type {
    SHARPNESS,  // Really Useful
    AVERAGE,    // Useful
    CONTOURS,   // Test
    EDGE,       // Test
    SOBEL,      // Test
    IDENTITY,
} Filter;

Image *filterImage(Image *image, Filter filter);

#endif //SRC_FILTER_H
