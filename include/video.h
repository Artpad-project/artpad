#ifndef SRC_VIDEO_H
#define SRC_VIDEO_H

#include "image.h"

typedef struct {
    int width, height;
    int frame_count;
    int fps;
    Image *frames;
}Video;

void free_video(Video video);
Video create_video(char *path, int w, int h, int fps);
void save_video(Video video, char *out);

#endif //SRC_VIDEO_H
