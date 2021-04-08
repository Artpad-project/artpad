#ifndef ROTOSCOPIE_H_
#define ROTOSCOPIE_H_

#include <sys/sysinfo.h>
#include <pthread.h>
#include <err.h>

#include "../image/image.h"
#include "../utils/utils.h"

#define MAGIC_WAND_THRESHOLD 0.125

ImageMask magic_wand(Image *im, int x, int y);

#endif // ROTOSCOPIE_H_