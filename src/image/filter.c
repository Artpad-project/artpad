/*!
 *  File created on 3/5/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  File containing the necessary functions to apply filters onto an image.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "filter.h"


//--------------------------------
void convolution(Image *image, Kernel (*f)());
void convolutePixel(Image *image, Kernel kernel, int x, int y);
void reverseKernel(Kernel kernel);
//--------------------------------
Kernel contoursFilter();
Kernel identityFilter();
Kernel edgeFilter();
Kernel sharpnessFilter();
Kernel sobelFilter();
Kernel averageFilter();
//--------------------------------

/*!
 * Apply a filter onto an image.
 * Main function to call outside the file.
 * @param image the image to apply the filter onto
 * @param filter the filter we want to apply
 */
Image *filterImage(Image *image, Filter filter) {
    image = copy_image(image);
    switch (filter) {
        case IDENTITY:
            convolution(image, identityFilter);
            break;
        case AVERAGE:
            convolution(image, averageFilter);
            break;
        case CONTOURS:
            convolution(image, contoursFilter);
            break;
        case SHARPNESS:
            convolution(image, sharpnessFilter);
            break;
        case EDGE:
            convolution(image, edgeFilter);
            break;
        case SOBEL:
            convolution(image, sobelFilter);
            break;
        default:
            ASSERT("Should not be reached.", image);
    }

    return image;
}

/*!
 * Apply convolution onto an image using a given kernel
 * @param image the image to apply convolution onto
 * @param f the function returning the kernel to use
 */
void convolution(Image *image, Kernel (*f)()) {
    Kernel kernel = (*f)();
    int center = kernel.size / 2;
    reverseKernel(kernel);

    for (int y = center; y < image->height - center; ++y) {
        for (int x = center; x < image->width - center; ++x) {
            convolutePixel(image, kernel, x, y);
        }
    }

    free(kernel.matrix);
}

/*!
 * Apply convolution to a given pixel
 * @param image the image we want to apply convolution to
 * @param kernel the kernel we use for the convolution (already reversed !)
 */
void convolutePixel(Image *image, Kernel kernel, int x, int y) {
    // place chosen pixel in the center of the kernel
    Pixel pixel = (Pixel){0, 0, 0, image->pixels[x][y].alpha};
    int center = kernel.size / 2;

    for (int i = 0; i < kernel.size; ++i) {
        for (int j = 0; j < kernel.size; ++j) {
            // multiplier les elements correpondants (k[i,j] *
            // pixels[x-1+i,y-1+j]) additioner tous les resultats des
            // multiplications
            int I = i - center, J = j - center;
            int value = kernel.matrix[(kernel.size * i) + j];
            Pixel tmp = image->pixels[x+J][y+I];
            pixel.blue += tmp.blue * value;
            pixel.red += tmp.red * value;
            pixel.green += tmp.green * value;
        }
    }

    image->pixels[x][y] = pixel;
}

/*!
 * Reverse the columns and the lines of a kernel (matrix).
 * [ [0,1],[2,3] ] => [ [3,2],[1,0] ]
 * @param kernel the kernel whose columns/lines we want to inverse
 */
void reverseKernel(Kernel kernel) {
    int total_size = kernel.size * kernel.size;
    for (int i = 0; i < total_size / 2;
         ++i) {  // swap doesn't work here idk why
        int a = kernel.matrix[i], b = kernel.matrix[total_size - i - 1];
        kernel.matrix[i] = b;
        kernel.matrix[total_size - i - 1] = a;
    }
}

/*!
 * Used as a test for the convolution algorithm
 * @return a kernel for contour filtering.
 */
Kernel identityFilter() {
    int *k = (int *)calloc(3 * 3, sizeof(int));
    k[4] = 1;
    Kernel res = (Kernel){k, 3};
    return res;
}

/*!
 * Used as a test for the convolution algorithm
 * @return a kernel for edge filtering.
 */
Kernel edgeFilter() {
    int *k = (int *)calloc(3 * 3, sizeof(int));
    k[0] = 1;
    k[8] = 1;
    k[2] = -1;
    k[6] = -1;
    Kernel res = (Kernel){k, 3};
    return res;
}
/*!
 * Used as a test for the convolution algorithm
 * @return a kernel for contour filtering.
 */
Kernel contoursFilter() {
    int *k = (int *)calloc(3 * 3, sizeof(int));
    for (int i = 0; i < 9; i++) k[i] = -1;
    k[4] = 8;
    Kernel res = (Kernel){k, 3};
    return res;
}


/*!
 * @return a kernel for accentuating contours.
 */
Kernel sharpnessFilter() {
    int *k = (int *)calloc(3 * 3, sizeof(int));

    // cross = -1
    k[1] = -1;
    k[3] = -1;
    k[5] = -1;
    k[7] = -1;
    // center = 5
    k[4] = 5;

    Kernel res = (Kernel){k, 3};
    return res;
}

/*!
 * Used as a test for the convolution algorithm
 * @return sobel's kernel.
 */
Kernel sobelFilter() {
    int *k = (int *)calloc(3 * 3, sizeof(int));

    k[0] = -1;
    k[2] = 1;
    k[3] = -2;
    k[5] = 2;
    k[6] = -1;

    Kernel res = (Kernel){k, 3};
    return res;
}

/*!
 * @return a kernel for filtering through average value.
 */
Kernel averageFilter() {
    int *k = (int *)calloc(3 * 3, sizeof(int));
    for (int i = 0; i < 9; ++i) k[i] = 1;

    Kernel res = (Kernel){k, 3};
    return res;
}
