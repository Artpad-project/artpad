#include <stdio.h>
#include "image.h"

int main() {

    gtk_init(0, NULL);

    gchar *filename = "shinobu.jpg";    
    Pixel black = {0, 0, 0, 255};

    struct Image *im = load_image(filename);

    // Fill the image with black
    for (int x = 0; x < im->width; ++x)
        for (int y = 0; y < im->height; ++y)
            im->pixels[x][y] = black;

    save_image(im, "output.png", NULL);

    free_image(im);

    struct Image *im2 = new_image(100,100);

    save_image(im2,"output2.png","png");
    free_image(im2);
    return 0;
}
