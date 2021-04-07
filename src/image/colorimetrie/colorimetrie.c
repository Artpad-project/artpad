#include <stdlib.h>
#include <err.h>
#include "colorimetrie.h"
#include "../image.h"
#include "stack.h"
#include <stdio.h>
#include <math.h>

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

void colorize(struct Image *img, struct Pixel color, int x, int y)
{
      img->pixels[x][y].red = color.red;
      img->pixels[x][y].green = color.green;
      img->pixels[x][y].blue = color.blue;
      img->pixels[x][y].alpha = color.alpha;
}

char IsInside(struct Image *img, int x, int y)
{
  return x >= 0 && x<img->width && y >= 0 && y<img->height;
}

void flood_fill(struct Image *img, struct Pixel color, int x, int y)
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
      colorize(img, color, x, y);

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

// Bresenham line drawing algorithm
// TODO Anti-Aliasing with WU algorithm
void paintLine(struct Image *img, struct Pixel color, int x1, int y1, int x2, int y2)
{
  int dx = abs(x2-x1);
  int sx = (x1<x2) ? 1 : -1;

  int dy = -abs(y2-y1);
  int sy = (y1<y2) ? 1 : -1;

  int err = dx+dy;
  while(1)
  {
    if (IsInside(img, x1, y1))
      colorize(img, color, x1, y1);
    if(x1 == x2 && y1 == y2)
      break;
    int e2 = 2*err;
    if(e2 >= dy)
    {
      err += dy;
      x1 += sx;
    }
    if(e2 <= dx)
    {
      err += dx;
      y1 += sy;
    }
  }
}

// draws each symmetrical point for each octants
void drawSymPoints(struct Image *img, struct Pixel color, int p, int q, int x, int y)
{
  int pos_x = x+p;
  int pos_y = y+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = y+p;
  pos_y = x+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = -y+p;
  pos_y = x+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = -x+p;
  pos_y = y+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = -x+p;
  pos_y = -y+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = -y+p;
  pos_y = -x+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = y+p;
  pos_y = -x+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

  pos_x = x+p;
  pos_y = -y+q;
  if (IsInside(img, pos_x, pos_y))
    colorize(img, color, pos_x, pos_y);

}

// Function that creates a circle following Bresenham algorithm
void circle(struct Image *img, struct Pixel color, int x, int y, int radius)
{
  radius-=1;
  if (IsInside(img, x, y) && radius == 0) colorize(img, color, x, y);
  else
  {
    int d = 3 - 2*radius;
    int x1 = 0;
    int y1 = radius;

    while (x1 <= y1) 
    {
      if (d <= 0) 
      {
        d += 4*x1 +6;
      }
      else 
      {
        d += 4*(x1-y1) + 10;
        y1-=1;
      }

      x1 += 1;
      drawSymPoints(img, color, x, y, x1, y1);
    }

    // Closes the circle
    int pos_x = x+radius;
    int pos_y = y;
    if (IsInside(img, pos_x, pos_y))
      colorize(img, color, pos_x, pos_y);

    pos_x = x-radius;
    pos_y = y;
    if (IsInside(img, pos_x, pos_y))
      colorize(img, color, pos_x, pos_y);

    pos_x = x;
    pos_y = y+radius;
    if (IsInside(img, pos_x, pos_y))
      colorize(img, color, pos_x, pos_y);

    pos_x = x;
    pos_y = y-radius;
    if (IsInside(img, pos_x, pos_y))
      colorize(img, color, pos_x, pos_y);
  }
}

/*void polygon(struct Pixel px, int x, int y)
{

}

void rectangle(struct Pixel px, int x, int y)
{

}

void cicle(struct Pixel px, int x, int y)
{

}
*/
