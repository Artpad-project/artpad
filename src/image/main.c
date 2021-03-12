#include <stdio.h>
#include "image.h"

int main() {

    gtk_init(0, NULL);

    gchar *filename = "test.png";    

    Image *im = load_image(filename);
    Image *copy = copy_image(im);
    ImageMask mask = init_mask(copy);

    // Fill the image with black
    for (int x = 0; x < im->width; ++x)
        for (int y = 0; y < im->height; ++y)
            if ((x + y) % 2)
                edit_mask(mask, x, y, TRUE);

    // Should save a copy of the image with vertical stripes
    save_image(mask.mask, "output.png", NULL);

    free_image(im);
    free_image(copy);
    free_mask(mask);

    return 0;
}
