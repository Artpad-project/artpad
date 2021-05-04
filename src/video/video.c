//
// Video processing example using FFmpeg
// Written by Ted Burke - last updated 12-2-2017
//

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "video.h"

// Video resolution
#define W 1280
#define H 720

// Allocate a buffer to store one frame
unsigned char frame[H][W][3] = {0};

Video create_video(char *path)
{
    DIR *frame_folder;
    Image *img, *frames;
    struct dirent *file;
    struct stat *st = malloc(sizeof(struct stat));
    Video video;

    if (stat("frames", st) == 0)
        printf("using already existing frames.\n");
    else {
        // Create folder
        char *cmd = malloc(256);
        sprintf(cmd, "ffmpeg -i %s ", path);
        cmd = strcat(cmd, "frames/frame_%03d.jpg");

        // Separate video into frames
        mkdir("frames", 0700);
        system(cmd);
        free(cmd);
    }

    // Read all frames
    frame_folder = opendir("frames");
    chdir("frames"); // Enter frames folder
    frames = malloc(sizeof(Image) * 500);
    video = (Video){W, H, 0, 60, NULL};
    while ((file = readdir(frame_folder))) {
        if (strcmp(file->d_name, ".") == 0 ||
            strcmp(file->d_name, "..") == 0)
            continue;
        img = load_image(file->d_name);
        frames[video.frame_count++] = *img;
    }

    free(img);
    video.frames = frames;

    printf("VIDEO SUCCESFULLY EXPORTED!\n");

    return video;
}

void free_video(Video video)
{
    for(int i = 0; i < video.frame_count; ++i)
        free_image(&video.frames[i]);
    free(video.frames);
}

int main(int argc, char** argv)
{
    int x, y, count;

    //int width = strtol(argv[1]);
    //int height= strtol(argv[2]);

    Video video = create_video("teapot.mp4");
    free_video(video);
    return 0;

    char *pipestr = malloc(256);
    sprintf(pipestr, "ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s %dx%d -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output.mp4", W, H);

    FILE *pipein = popen("ffmpeg -i teapot.mp4 -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");
    FILE *pipeout = popen(pipestr, "w");

    // Process video frames
    while(1)
    {
        // Read a frame from the input pipe into the buffer
        count = fread(frame, 1, H*W*3, pipein);

        // If we didn't get a frame of video, we're probably at the end
        if (count != H*W*3) break;

        // Process this frame
        for (y=0 ; y<H ; ++y) for (x=0 ; x<W ; ++x)
            {
                // Invert each colour component in every pixel
                frame[y][x][0] = 255 - frame[y][x][0]; // red
                frame[y][x][1] = 255 - frame[y][x][1]; // green
                frame[y][x][2] = 255 - frame[y][x][2]; // blue
            }

        // Write this frame to the output pipe
        fwrite(frame, 1, H*W*3, pipeout);
    }

    // Flush and close input and output pipes
    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);

    return 1;
}
