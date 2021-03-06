//
// Created by titalah on 04/04/2021.
//

#include <stdio.h>
#include "../../include/colorimetrie.h"
#include <string.h>


/* argv[1]: function
 * argv[2]: loading path
 * argv[3]: saving path
 * argv[4]: radius (for circle)
 * argv[5]: filling
 * argv[6]: acceptance (for flood fill)
 * argv[7]: size (for paint line)
 */
void CLI(char *command, int argc, struct Image *img, struct Pixel color, 
    struct coord c, int radius, int filled, int acceptance, int size)
{
  if (!strcmp(command, "-r"))
  {
    struct coord c1 = {c.x-50, c.y-50};
    struct coord c2 = {c.x+50, c.y+50};
    rectangle(img, color, c1, c2, filled);
  }
  
  else if (!strcmp(command, "-c"))
    circle(img, color, c, radius, filled);
  
  else if (!strcmp(command, "-l"))
  {
    struct coord src = {0, 0};
    struct coord dest = {img->width, img->height};
    paintLine(img, color, src, dest, size);
  }

  else if (!strcmp(command, "-f"))
    flood_fill(img, color, c, acceptance);
}

int main(int argc, char **argv)
{
    //if (argc != 4)
        //errx(EXIT_FAILURE, "Only two argument are asked\n");

    struct Image *img = load_image(argv[2]);

    int x = img->width/2;
    int y = img->height/2;
    struct coord c = {x, y};
    struct Pixel px = {0,0,255,0};

    CLI(argv[1], argc, img, px, c, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

    //char output[150];
    //strcat(output, "images/base/");
    //strcat(output, argv[3]);

    save_image(img, argv[3], "jpeg");
    free_image(img);
    free(img);

    return 0;
}
