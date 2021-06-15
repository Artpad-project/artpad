#ifndef ROTOSCOPIE_H_
#define ROTOSCOPIE_H_

#include <sys/sysinfo.h>
#include <pthread.h>
#include <err.h>

#include "image.h"
#include "utils.h"
#include "video.h"

#define MAGIC_WAND_THRESHOLD 0.250

ImageMask magic_wand(Image *im, int x, int y);
void rotoscopie(Video video, int x, int y, int *percent);

#endif // ROTOSCOPIE_H_
