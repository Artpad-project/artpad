#include <stdio.h>
#include "../../include/rotoscopie.h"
#include "../../include/image.h"
#include "../../include/video.h"

int main(int argc, char *argv[])
{
    Video video;
    int frame_count;
    char title[512];

    if (argc != 7)
        err(1, "Usage: ./video 'video_file' width height fps x y\n");

    gtk_init(0, NULL);
    int percent = 0;

    // https://drive.google.com/open?id=0B3NaVR72FYQcWUFSUVhKWE5UMVU
    // Local: "teapot.png" 1280 720 25 600 300
    video = create_video(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

    sprintf(title,  "out/test_video_%d.mp4", video.frame_count);
    save_video(video, title);

    free_video(video);

    return 0;
}
