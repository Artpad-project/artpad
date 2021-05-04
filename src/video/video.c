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

size_t get_frame_number(char *file)
{
    size_t frame_nbr = 0;
    file += 6; // Skip the 'frame_' part of the filename

    while (*file != '.') {
        frame_nbr *= 10;
        frame_nbr += *file - '0';
        ++file;
    }

    return frame_nbr - 1; // Frames start at 1
}

Video create_video(char *path)
{
    DIR *frame_folder;
    Image *img, *frames;
    struct dirent *file;
    struct stat *st = malloc(sizeof(struct stat));
    Video video;
    int total_frame_count = -2; // minus '.' and '..'

    if (stat("frames", st) == 0)
        printf("using already existing frames.\n");
    else {
        // Create folder
        char *cmd = malloc(256);
        sprintf(cmd, "ffmpeg -i %s ", path);
        cmd = strcat(cmd, "frames/frame_%04d.jpg");

        // Separate video into frames
        mkdir("frames", 0700);
        system(cmd);
        free(cmd);
    }

    // Find the actual number of frames
    frame_folder = opendir("frames");
    while ((file = readdir(frame_folder))) ++total_frame_count;

    // Read all frames
    frame_folder = opendir("frames");
    chdir("frames"); // Enter frames folder
    frames = malloc(sizeof(Image) * total_frame_count);
    video = (Video){W, H, 0, 60, NULL};
    // Load every frame as an image
    while ((file = readdir(frame_folder))) {
        if (strcmp(file->d_name, ".") == 0 ||
            strcmp(file->d_name, "..") == 0)
            continue;
        img = load_image(file->d_name);
        frames[get_frame_number(file->d_name)] = *img;
        ++video.frame_count;
    }

    free(img);
    video.frames = frames;

    chdir("..");
    printf("VIDEO SUCCESFULLY EXPORTED!\n");

    return video;
}

void free_video(Video video)
{
    for(int i = 0; i < video.frame_count; ++i)
        free_image(&video.frames[i]);
    free(video.frames);
}

void save_video(Video video, char *out)
{
    char *pipestr = malloc(256);

    sprintf(pipestr,
            "ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s %dx%d "
            "-r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 %s",
            video.width, video.height, out);

    FILE *pipeout = popen(pipestr, "w");

    // Allocate a buffer to store one frame
    unsigned char frame[H][W][3] = {0};

    for (int i = 0; i < video.frame_count; ++i) {
        for (int y=0 ; y<video.height ; ++y) for (int x=0 ; x<video.width ; ++x) {
            frame[y][x][0] = video.frames[i].pixels[x][y].red;
            frame[y][x][1] = video.frames[i].pixels[x][y].green;
            frame[y][x][2] = video.frames[i].pixels[x][y].blue;
        }

        // Write this frame to the output pipe
        fwrite(frame, 1, video.width*video.height*3, pipeout);
    }

    // Flush and close output pipe
    fflush(pipeout);
    pclose(pipeout);

    printf("VIDEO SUCCESFULLY SAVED!\n");
}

int main(int argc, char** argv)
{
    Video video;

    video = create_video("teapot.mp4");
    save_video(video, "output.mp4");

    return 1;
}
