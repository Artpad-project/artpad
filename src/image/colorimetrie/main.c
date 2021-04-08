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

    //flood_fill(img, px, x, y);
    //paintLine(img, px, 0, 0, img->width, img->height);
    //circle(img, px, x, y, 100, 1);
    //rectangle(img, px, x-50, y-50, x+50, y+50, 1);

    //char output[100];
    //strcat(output, "images/results/");
    //strcat(output, argv[2]);

    save_image(img, "images/results/line", "jpeg");
    free_image(img);

    return 0;
}
