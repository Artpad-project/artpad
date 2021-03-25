#include <stdio.h>
#include "image.h"
#include "filter.h"

int main() {

    gtk_init(0, NULL);

    gchar *filename = "mountain.jpg";    

    Image *im = load_image(filename);
    Image *copy = filterImage(im, EDGE);

    // Should save a copy of the image with vertical stripes
    save_image(copy, "output.png", NULL);

    free_image(im);
    free_image(copy);

    return 0;
}
