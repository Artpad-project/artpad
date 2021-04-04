#include <stdlib.h>
#include <err.h>
#include "colorimetrie.h"
#include <stdio.h>

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define TRUE 1
#define FALSE 0

void flood_fill(int **pixels, int x, int y, int val, int width, int height, int dir)
{
  int oldval = pixels[x][y];
  printf("x: %d, y: %d\n", x, y);

  pixels[x][y] = val;

  if (dir != UP && x+1 < width && pixels[x+1][y] == oldval)
    flood_fill(pixels, x+1, y, val, width, height, DOWN);

  if (dir != DOWN && x-1 >= 0 && pixels[x-1][y] == oldval)
    flood_fill(pixels, x-1, y, val, width, height, UP);

  if (dir != LEFT && y+1 < width && pixels[x][y+1] == oldval)
    flood_fill(pixels, x, y+1, val, width, height, RIGHT);

  if (dir != RIGHT && y-1 < width && pixels[x][y-1] == oldval)
    flood_fill(pixels, x, y-1, val, width, height, LEFT);
}
