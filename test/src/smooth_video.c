//
// Created by leo on 6/16/21.
//

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "../../include/video.h"

int main(int argc, char *argv[])
{
    Video video;
    Video smooth;
    char title[512];

    if (argc != 7)
        err(1, "Usage: ./video 'video_file' width height fps x y\n");

    gtk_init(0, NULL);

    // https://drive.google.com/open?id=0B3NaVR72FYQcWUFSUVhKWE5UMVU
    // Local: "teapot.png" 1280 720 30 600 300
    video = create_video(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    smooth = smooth_video(video);

    sprintf(title, "out/out_smooth_%dfps.mp4", smooth.fps);
    save_video(smooth, title);

    //free_video(video);

    return 0;
}
