/*!
 *  File created on 3/10/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  FPS smoothing using Motion interpolation.
 *
 *  Motion interpolation: https://monochrome.sutic.nu/2010/10/12/motion-interpolation.html
 */

#include "../../include/video.h"
#include "../../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <float.h>

#define BOX_SIZE 5
#define FLOW_DISTANCE 5

/*

block = crop (firstFrame, x, y, x+s, y+s);
bestSum = MAX_FLOAT;
bestPosition = [x,y];
for (int dx = -d; dx < d; ++dx) {
    for (int dy = -d; dy < d; ++dy) {
        correspondingBlock = crop (secondFrame, x+dx, y+dy, x+s+dx, y+s+dy);
        difference = subtract (block, correspondingBlock);
        sum = 0
        foreach pixel p in difference:
            sum = sum + sqr (p)
        if (sum < bestSum)
            bestSum = sum
            bestPosition = [x+dx,y+dy]
    }
}
opticalFlow (x,y) = bestPosition - [x,y]

*/

#define SQR(x) (x*x)

struct coord {
    int x;
    int y;
};

struct coord compute_vector_flow(Frame f1, int x, int y, Frame f2)
{
    float best_sum = FLT_MAX;
    struct coord best_pos = (struct coord) {x, y};
    Pixel p;
    float sum;

    if (x < FLOW_DISTANCE || y < FLOW_DISTANCE ||
        x >= f1.width - FLOW_DISTANCE || y >= f1.height - FLOW_DISTANCE)
        return (struct coord) {best_pos.x - x, best_pos.y - y};

    for (int dx = -FLOW_DISTANCE; dx < FLOW_DISTANCE; ++dx) {
        for (int dy = -FLOW_DISTANCE; dy < FLOW_DISTANCE; ++dy) {
            sum = 0;
            for (int X = x+dx; X < x+BOX_SIZE+dx; X++) {
                for (int Y = y+dy; Y < y+BOX_SIZE+dy; Y++) {
                    // FIXME: SEGFAULT y = 712
                    p = DIF_PX(f1.pixels[X][Y], f2.pixels[X][Y]);
                    sum += SQR(SUM_PX(p));
                }
            }
            if (sum < best_sum) {
                best_sum = sum;
                best_pos = (struct coord) {x+dx, y+dy};
            }
        }
    }

    return (struct coord) {best_pos.x - x, best_pos.y - y};
}

#define SMOOTH_FACTOR 2

Video smooth_video(Video video)
{
    printf("Entered %s\n", __func__ );

    Video smooth_video;
    Frame f1, f2;
    struct coord vector_flow;
    float t_i = 0.5; // time factor used for interpolation
    int xb, yb;
    int xf, yf;

    smooth_video = video;
    smooth_video.fps = video.fps * SMOOTH_FACTOR;
    smooth_video.frame_count = video.frame_count * ((float)smooth_video.fps / (float)video.fps) - 1;
    smooth_video.frames = malloc(smooth_video.frame_count * sizeof(Frame));

    //TODO: speed this up (please)
    memset(smooth_video.frames, 0, smooth_video.frame_count * sizeof(Frame));
    for (int i = smooth_video.frame_count - 1; i >= 0; i -= 2) {
        Frame *frame = create_copy_image(&video.frames[i/2]);
        smooth_video.frames[i] = *frame;
        free(frame);
    }
    free_video(video);


    for (int frame = 0; frame < smooth_video.frame_count-2; frame+=2) {
        printf("-- processing frame %d/%d\n", 2*frame+1, smooth_video.frame_count);
        f1 = smooth_video.frames[frame];
        f2 = smooth_video.frames[frame+2];
        Image *im = new_image(video.width, video.height);
        for (int x = 0; x < video.width; ++x) {
            for (int y = 0; y < video.height; ++y) {
                printf("(%d,%d) \n", x, y);
                vector_flow = compute_vector_flow(f1, x, y, f2);
                xb = x - t_i*vector_flow.x;
                yb = y - t_i*vector_flow.y;
                xf = x + (1 - t_i)*vector_flow.x;
                yf = y + (1 - t_i)*vector_flow.y;
                im->pixels[x][y] = MULT_PX(f1.pixels[xb][yb], (1-t_i));
                im->pixels[x][y] = ADD_PX(
                        MULT_PX(f2.pixels[xf][yf], t_i),
                        im->pixels[x][y]);
            }
        }
        smooth_video.frames[2*frame] = f1;
        smooth_video.frames[2*frame+1] = *im;
        smooth_video.frames[2*(frame+1)] = f2;
        free(im);
    }

    return smooth_video;
}
