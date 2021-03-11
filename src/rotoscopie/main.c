#include <stdio.h>
#include "../image/image.h"
#include "rotoscopie.h"

int main(int argc, char *argv[]) {

    if (argc != 4)
        errx(1, "Invalid number of args.\n");

    gtk_init(0, NULL);

    struct Image *im = load_image(argv[1]);
    struct Image *copy = magic_wand(im, atoi(argv[2]), atoi(argv[3]));

    save_image(copy, "output.png", NULL);

    free_image(im);
    free_image(copy);

    return 0;
}
