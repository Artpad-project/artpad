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

void paint();
void polygon();
void circle();
void rectangle();
void change_color(int *color); //TODO add GTK interactions in parameter
void change_width(int *width); //TODO same as above
void flood_fill(struct Image *img, int x, int y, struct Pixel color);

#endif
