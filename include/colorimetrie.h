/*
 *  File created on 25/03/2021 by lowen.desmarais
 *  Contributors: lowen.desmarais
 *  
 *  File containing all necessary functions prototypes to colorize and paint an
 *  image. 
 *  These include basic interactions as well as the flood_fill algorithm
 */

#ifndef COLORIMETRIE_H_
#define COLORIMETRIE_H_

#include <stdlib.h>
#include <err.h>
#include "image.h"
#include "stack.h"

void circle(struct Image *img, struct Pixel color, struct coord center, int radius, int filled);
void rectangle(struct Image *img, struct Pixel color, struct coord c1, struct coord c2
    , int filled);
void paintLine(struct Image *img, struct Pixel color, struct coord src, struct coord dest, 
    int size);
void pencil(struct Image *img, struct Pixel color, struct coord src, struct coord dest,
    int size);
void brush(struct Image *img, struct Pixel color, struct coord src, struct coord dest,
    int size);
void special_brushes(struct Image *img, struct Pixel color, struct coord src, struct coord dest,
    int size);
void colorize(struct Image *img, struct Pixel color, int x, int y);
void flood_fill(struct Image *img, struct Pixel color, struct coord c, int acceptance);

#endif
