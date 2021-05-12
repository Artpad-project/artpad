#include <err.h>
#include "../../include/video.h"

int main(int argc, char** argv)
{
    if (argc != 5)
        err(1, "Usage: ./video 'video_file' width height fps\n");

    Video video;

    // https://drive.google.com/open?id=0B3NaVR72FYQcWUFSUVhKWE5UMVU
    // Local: "teapot.png" 1280 720 25
    video = create_video(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    save_video(video, "output.mp4");

    free_video(video);

    return 0;
}
