#ifndef SRC_VIDEO_H
#define SRC_VIDEO_H

#include "image.h"

typedef Image Frame;

typedef struct Video{
    int width, height;
    int frame_count;
    int fps;
    Frame *frames;
}Video;

void free_video(Video video);
Video create_video(char *path, int w, int h, int fps);
void save_video(Video video, char *out);
Video smooth_video(Video video);

typedef Image Frame;

#endif //SRC_VIDEO_H
