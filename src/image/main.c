#include <stdio.h>
#include "image.h"

int main() {

    gtk_init(0, NULL);

    gchar *filename = "test.jpg";    

    struct Image *im = load_image(filename);
    save_image(im);

    free_image(im);
    return 0;
}
