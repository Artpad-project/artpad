#ifndef SRC_VIDEO_H
#define SRC_VIDEO_H

#include "../image/image.h"

#include <gstreamer-1.0/gst/gst.h>

typedef struct {
    int width, height;
    int frame_count;
    int fps;
    Image *frames;
}Video;

#endif //SRC_VIDEO_H
