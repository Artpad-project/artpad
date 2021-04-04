#include <stdlib.h>
#include <stdio.h>
#include "colorimetrie.h"
#include <err.h>

void pretty_print(int **pixels, int width, int height)
{
  for (int x = 0; x<width; x++)
  {
    for (int y = 0; y < height; y++)
      printf(" %3d,", pixels[x][y]);
    printf("\n");
  }
}

int main(int argc, char **argv)
{
  if (argc != 2)
    errx(EXIT_FAILURE, "Only one arguments should be passed: (int) color\n");

  int val = atoi(argv[1]);
  int width = 255;
  int height = 255;

  int **pixels = malloc(width * sizeof(int *));
  for(int i = 0; i < width; i++)
    pixels[i] = malloc(height * sizeof(int));


  for (int x = 0; x < width; x++)
    for(int y = 0; y < height; y++)
    {
      if (x > width/2 -2 && x < width/2 +2 && y > height/2 -2 && y < height/2 +2) 
        pixels[x][y] = 255/2;

      else
        pixels[x][y] = rand()%(255/2);
    }


  printf("base pixels\n");
  pretty_print(pixels, width, height);

  printf("flooding pixels\n");
  flood_fill(pixels, width/2, height/2, val, width, height, NULL);

  printf("flooded pixels\n");
  pretty_print(pixels, width, height);

  for(int i = 0; i < width; i++)
    free(pixels[i]);
  free(pixels);

  return 0;
}
