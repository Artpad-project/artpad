//
// Created by titalah on 04/04/2021.
//

#include <stdio.h>
#include "colorimetrie.h"
#include <string.h>

int main(int argc, char **argv)
{
    //if (argc != 4)
        //errx(EXIT_FAILURE, "Only two argument are asked\n");

    struct Image *img = load_image(argv[1]);

    int x = img->width/2;
    int y = img->height/2;
    struct Pixel px = {0,0,255,0};

    //flood_fill(img, x, y, px);
    //paint(img, px, x, y, atoi(argv[3]));
    paintLine(img, px, 0, 0, img->width, img->height);

    char output[80];
    strcat(output, "images/results/");
    strcat(output, argv[2]);

    save_image(img, output, "jpeg");
    free_image(img);

    return 0;
}
