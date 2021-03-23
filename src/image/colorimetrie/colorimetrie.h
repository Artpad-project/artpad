#ifndef COLORIMETRIE_H_
#define COLORIMETRIE_H_

#include <stdlib.h>
#include <err.h>
#include "../image.h"

void colorize(const struct Pixel px, int x, int y, int color, int width);
void change_color(int *color); //TODO add GTK interactions in parameter
void change_width(int *width); //TODO same as above

#endif
