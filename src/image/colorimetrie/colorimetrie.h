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
#include "../image.h"

void circle(struct Image *img, struct Pixel color, int x, int y, int radius);
//void polygon();
//void circle();
//void rectangle();
void colorize(struct Image *img, struct Pixel color, int x, int y);
void flood_fill(struct Image *img, struct Pixel color, int x, int y);
void paintLine(struct Image *img, struct Pixel color, int x1, int y1, int x2, int y2);

#endif
