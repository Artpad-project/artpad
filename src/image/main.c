#include <stdio.h>
#include "image.h"
#include "filter.h"
#include "image_scaling.h"

int main() {

    gtk_init(0, NULL);

    char *filename = "test.png";

    Image *im = load_image(filename);

/* // Fill the image with black
    for (int x = 0; x < im->width; ++x)
        for (int y = 0; y < im->height; ++y)
            im->pixels[x][y] = black;
*/
    save_image(im, "output.png", NULL);

    struct Image *im2 = rescale_image(im,50);

    save_image(im2,"output2.png","png");
    free_image(im);

    free_image(im2);
    return 0;
}
