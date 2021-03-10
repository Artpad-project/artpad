#include <stdio.h>
#include "image.h"

int main() {

    gtk_init(0, NULL);

    gchar *filename = "test.png";    

    Image *im = load_image(filename);
    Image *copy = copy_image(im);

    // Fill the image with black
    for (int x = 0; x < im->width; ++x)
        for (int y = 0; y < im->height; ++y)
            copy->pixels[x][y] = BLACK;

    save_image(copy, "output.png", NULL);

    free_image(im);
    free_image(copy);

    return 0;
}
