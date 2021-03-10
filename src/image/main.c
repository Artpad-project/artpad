#include <stdio.h>
#include "image.h"

int main() {

    gtk_init(0, NULL);

    gchar *filename = "test.jpg";    
    Pixel black = {0, 0, 0};

    struct Image *im = load_image(filename);

    // Fill the image with black
    for (int x = 0; x < im->width; ++x)
        for (int y = 0; y < im->height; ++y)
            im->pixels[x][y] = black;

    save_image(im, NULL, NULL);
    free_image(im);

    return 0;
}
