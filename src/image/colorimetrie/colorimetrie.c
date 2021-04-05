#include <stdlib.h>
#include <err.h>
#include "colorimetrie.h"
#include "../image.h"
#include "stack.h"
#include <stdio.h>

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

void flood_fill(struct Image *img, int x, int y, struct Pixel color)
{
  struct Pixel px = img->pixels[x][y];

  //printf("creating stack\n");
  stack *s = new_stack();
  struct coord c = {x, y};

  //printf("pushing corrdinates\n");
  stack_push(s, c);

  while(!stack_IsEmpty(s))
  {
      //printf("popping stack\n");
      c = stack_pop(s);
      int x = c.x;
      int y = c.y;

      //printf("coloring pixel\n");
      img->pixels[x][y].red = color.red;
      img->pixels[x][y].green = color.green;
      img->pixels[x][y].blue = color.blue;
      img->pixels[x][y].alpha = color.alpha;

      //printf("starting to fill queue\n");
      if (x+1 < img->width && same_color(img->pixels[x+1][y], px))
      {
          struct coord new_c = {x+1, y};
          stack_push(s, new_c);
      }

      if (x-1 >= 0 && same_color(img->pixels[x-1][y], px))
      {
          struct coord new_c = {x-1, y};
          stack_push(s, new_c);
      }

      if (y+1 < img->height && same_color(img->pixels[x][y+1], px))
      {
          struct coord new_c = {x, y+1};
          stack_push(s, new_c);
      }

      if (y-1 >= 0 && same_color(img->pixels[x][y-1], px))
      {
          struct coord new_c = {x, y-1};
          stack_push(s, new_c);
      }

      //printf("finished pushing stack\n");
  }

  //printf("freeing stack\n");
  stack_free(s);

}

void paint()
{

}

void polygon()
{

}

void rectangle()
{

}

void cicle()
{

}