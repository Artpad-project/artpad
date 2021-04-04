#include <stdlib.h>
#include <err.h>
#include "colorimetrie.h"
#include "../image.h"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

char same_color(struct Pixel px, struct Pixel color)
{
  return px.red == color.red &&
      px.blue == color.blue &&
      px.green == color.green &&
      px.alpha == color.alpha;
}

void flood_fill(struct Image *img, int x, int y, struct Pixel color, int dir)
{
  struct Pixel px = img->pixels[x][y];
//  printf("x: %d, y: %d\n", x, y);

  img->pixels[x][y].red = color.red;
  img->pixels[x][y].green = color.green;
  img->pixels[x][y].blue = color.blue;
  img->pixels[x][y].alpha = color.alpha;

  if(dir != UP && x+1 < img->width && same_color(img->pixels[x+1][y], px))
    flood_fill(img, x+1, y, color, DOWN);

  if(dir != DOWN && x-1 >= 0 && same_color(img->pixels[x-1][y], px))
    flood_fill(img, x-1, y, color, UP);

  if(dir != LEFT && y+1 < img->height && same_color(img->pixels[x][y+1], px))
    flood_fill(img, x, y+1, color, RIGHT);

  if(dir != RIGHT && y-1 >= 0 && same_color(img->pixels[x][y-1], px))
    flood_fill(img, x, y-1, color, LEFT);
}
