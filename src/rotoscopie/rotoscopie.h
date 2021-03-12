#ifndef ROTOSCOPIE_H_
#define ROTOSCOPIE_H_

#include "../image/image.h"
#include <sys/sysinfo.h>
#include <pthread.h>
#include <err.h>

#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAGIC_WAND_THRESHOLD 0.125

ImageMask magic_wand(Image *im, int x, int y);

#endif // ROTOSCOPIE_H_