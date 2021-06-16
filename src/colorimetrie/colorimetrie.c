#include <stdlib.h>
#include <err.h>
#include "../../include/colorimetrie.h"
#include "../../include/image.h"
#include <math.h>

typedef struct coord coord;

char same_color(struct Pixel px, struct Pixel origin, int acceptance)
{
    acceptance = 255*acceptance / 100;
    return  ABS(origin.red - px.red) <= acceptance &&
            ABS(origin.green - px.green) <= acceptance &&
            ABS(origin.blue - px.blue) <= acceptance;
}

char filled(struct Pixel px, struct Pixel origin)
{
  return same_color(px, origin, 0);
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

void copy_buffer(struct Image *img, struct Image *buffer, struct coord origin)
{
  struct Pixel white = {255, 255, 255, 255};
  for(int i = 0; i < buffer->width; i++)
    for(int j = 0; j < buffer->height; j++)
      if (!same_color(img->pixels[i][j], white, 0))
        colorize(img, buffer->pixels[i][j], origin.x+i, origin.y+j);
}

/* content for scanlines but overfills
          if (!flag && !same_color(img->pixels[x][y+1], px, acceptance))
            flag = 1;

          if (flag && same_color(img->pixels[x][y+1], px, acceptance))
          {
            struct coord new_c = {x, y+1};
            stack_push(s, new_c);
            flag = 0;
          }

          flag = 0;

          if (!flag && !same_color(img->pixels[x][y-1], px, acceptance))
            flag = 1;

          if (flag && same_color(img->pixels[x][y-1], px, acceptance))
          {
            struct coord new_c = {x, y-1};
            stack_push(s, new_c);
            flag = 0;
          }


          x+=1;
*/

void flood_fill(struct Image *img, struct Pixel color, struct coord origin, int acceptance)
{
  int x = origin.x;
  int y = origin.y;
  struct Pixel px = img->pixels[x][y];

  if (acceptance < 100 && same_color(px, color, acceptance))
    return;

  ff_stack *s = new_stack();

  struct ff_coord first = {x, x, y, 1};
  struct ff_coord second = {x, x , y-1, -1};

  stack_push(s, first);
  stack_push(s, second);

  unsigned char *buf = malloc(img->width*img->height);
  memset(buf,0,img->width*img->height);

  struct ff_coord c;
  while(!stack_IsEmpty(s))
  {
      //printf("popping stack\n");
      c = stack_pop(s);
      int x1 = c.x1;
      int x2 = c.x2;
      int y = c.y;
      char dy = c.dy;
      
      if (buf[y*img->width+x])
        continue;
      buf[y*img->width+x] = 1;

      int x = x1;
      if (IsInside(img, x, y))
      {
        while (x-1 >= 0 && same_color(img->pixels[x-1][y], px, acceptance))
        {
          colorize(img, color, x, y);
          x -= 1;
        }
      }

      if (x < x1)
      {
        struct ff_coord new_c = {x, c.x1-1, y-dy, -dy};
        stack_push(s, new_c);
      }

      while (x1 < x2)
      {
        while(x1 <= img->width && same_color(img->pixels[x1+1][y], px, acceptance))
        {
          colorize(img, color, x1, y);
          x1 += 1;
        }

        struct ff_coord new_c = {x, x1 -1, y+dy, dy};
        stack_push(s, new_c);

        if (x1-1 > x2)
        {
          struct ff_coord new_c = {x2+1, x1-1, y-dy, -dy};
          stack_push(s, new_c);
        }

        while (x1<x2 && x1 < img->width && !filled(img->pixels[x1+1][y], px))
          x1 += 1;

        x = x1;
      }

      /*
      //printf("coloring pixel\n");
      colorize(img, color, x, y);

      //printf("starting to fill queue\n");
      if (x+1 < img->width && same_color(img->pixels[x+1][y], px, acceptance))
      {
          struct coord new_c = {x+1, y};
          stack_push(s, new_c);
      }

      if (x-1 >= 0 && same_color(img->pixels[x-1][y], px, acceptance))
      {
          struct coord new_c = {x-1, y};
          stack_push(s, new_c);
      }

      if (y+1 < img->height && same_color(img->pixels[x][y+1], px, acceptance))
      {
          struct coord new_c = {x, y+1};
          stack_push(s, new_c);
      }

      if (y-1 >= 0 && same_color(img->pixels[x][y-1], px, acceptance))
      {
          struct coord new_c = {x, y-1};
          stack_push(s, new_c);
      }
      */
  }

  //printf("freeing stack\n");
  stack_free(s);
  free(buf);
}

// Bresenham line drawing algorithm
// TODO Anti-Aliasing with WU algorithm
void paintLine(struct Image *img, struct Pixel color, struct coord src, struct coord dest, 
    int size)
{
  int x1 = src.x;
  int y1 = src.y;
  int x2 = dest.x;
  int y2 = dest.y;

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


void brush(struct Image *img, struct Pixel color, struct coord src, struct coord dest, int size)
{
  int x1 = src.x;
  int y1 = src.y;
  int x2 = dest.x;
  int y2 = dest.y;

  int dx = abs(x2-x1);
  int sx = (x1<x2) ? 1 : -1;

  int dy = -abs(y2-y1);
  int sy = (y1<y2) ? 1 : -1;

  int err = dx+dy;
  while(1)
  {
    if (IsInside(img, x1, y1))
      colorize(img, color, x1, y1);

    if (size)
    {
      for (int i = 0; i < size; i++)
      {
        if (IsInside(img, x1, y1+i))
          colorize(img, color, x1, y1+i);

        if (IsInside(img, x1, y1-i))
          colorize(img, color, x1, y1-i);
      }
    }

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

void special_brushes(struct Image *img, struct Pixel color, struct coord src, 
    struct coord dest, int offset)
{
  struct coord copy_src = {src.x, src.y};
  struct coord copy_dest = {dest.x, dest.y};
  int side = (offset < 0) ? -1 : 1;
  Pixel shadow = {color.red, color.blue, color.green, color.alpha-10};
  for(int i = 0; i < abs(offset); i++)
  {
    src.x += i;
    src.y += i;
    dest.x += i;
    dest.y += i;
    paintLine(img, color, src, dest, 0);

    // can change vaalues here to create some sort of shade
    src.x = copy_src.x + i;
    src.y = copy_src.y +i*side;
    dest.x = copy_dest.x + i;
    dest.y = copy_dest.y + i*side;
    paintLine(img, shadow, src, dest, 0);
  }
}

void pencil(struct Image *img, struct Pixel color, struct coord src, struct coord dest, int size)
{
  int x1 = src.x;
  int y1 = src.y;
  int x2 = dest.x;
  int y2 = dest.y;

  int dx = abs(x2-x1);
  int sx = (x1<x2) ? 1 : -1;

  int dy = -abs(y2-y1);
  int sy = (y1<y2) ? 1 : -1;

  int err = dx+dy;

  while(1)
  {
    // spacing of 25% diameter of the circle
    if (IsInside(img, x1, y1) /*&& (pow(src.x-x1, 2) + pow(src.y-y1, 2) <= pow(25*size/100, 2))*/)
    {
      struct coord center = {x1, y1};
      circle(img, color, center, size, 1);
    }

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
void drawSymPoints(struct Image *img, struct Pixel color, struct coord center, 
    struct coord points, int filled)
{
  int p = center.x;
  int q = center.y;
  int x = points.x;
  int y = points.y;

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

  if (filled)
  {
    struct coord c1 = {x+p, y+q};
    struct coord c2 = {-x+p, -y+q};

    struct coord c3 = {x+p, -y+q};
    struct coord c4 = {-x+p, y+q};

    struct coord c5 = {y+p, -x+q};
    struct coord c6 = {-y+p, x+q};

    struct coord c7 = {-y+p, -x+q};
    struct coord c8 = {y+p, x+q};

    // connecting lines with same y coordinates
    paintLine(img, color, c1, c4, 0);
    paintLine(img, color, c6, c8, 0);
    paintLine(img, color, c2, c3, 0);
    paintLine(img, color, c5, c7, 0);
  }
}

// Function that creates a circle following Bresenham algorithm
void circle(struct Image *img, struct Pixel color, struct coord center, int radius, int filled)
{
  int x1 = center.x;
  int y1 = center.y;

  if (IsInside(img, x1, y1) && radius == 0) colorize(img, color, x1, y1);
  else
  {
    int d = 3 - 2*radius;
    int x1 = 0;
    int y1 = radius;

    while (x1 <= y1) 
    {
      if (d <= 0) 
        d += 4*x1 +6;

      else 
      {
        d += 4*(x1-y1) + 10;
        y1-=1;
      }

      x1 += 1;
      
      struct coord c = {x1, y1};
      
      drawSymPoints(img, color, center, c, filled);
    }

    // Closes the circle
    // same as symmety point draw
    if (filled)
    {
      struct coord left = {center.x+radius, center.y};
      struct coord right = {center.x-radius, center.y};
      paintLine(img, color, left, right, 0);

      left.x = center.x;
      left.y = center.y+radius;
      right.x = center.x;
      right.y = center.y-radius;
      paintLine(img, color, left, right, 0);
    }
    else
    {
      if (!IsInside(img, center.x+radius, center.y))
        colorize(img, color, center.x+radius, center.y);

      if (!IsInside(img, center.x-radius, center.y))
        colorize(img, color, center.x-radius, center.y);

      if (!IsInside(img, center.x, center.y+radius))
        colorize(img, color, center.x, center.y+radius);

      if (!IsInside(img, center.x, center.y-radius))
        colorize(img, color, center.x, center.y-radius);
    }
  }
}

void rectangle(struct Image *img, struct Pixel color, struct coord c1, struct coord c2,
    int filled)
{
  int x1 = c1.x;
  int y1 = c1.y;
  int x2 = c2.x;
  int y2 = c2.y;

  if (filled)
  {
    for(; x1 <= x2; x1++)
    {
      for(; y1 <= y2; y1++)
        if (IsInside(img, x1, y1))
          colorize(img, color, x1, y1);
      y1 = c1.y;
    }
  }
  else
  {
    while(y1 <= y2)
    {
      if (IsInside(img, x1, y1))
        colorize(img, color, x1, y1);

      if (IsInside(img, x2, y1))
        colorize(img, color, x2, y1);
      y1++;
    }

    y1 = c1.y;

    while(x1 <= x2)
    {
      if (IsInside(img, x1, y1))
        colorize(img, color, x1, y1);

      if (IsInside(img, x1, y2))
        colorize(img, color, x1, y2);

      x1++;
    }
  }
} 

