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
#include <sys/sysinfo.h>
#include <pthread.h>

#define BOX_SIZE 3
#define FLOW_DISTANCE 3

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
        x >= f1.width - FLOW_DISTANCE - BOX_SIZE || y >= f1.height - FLOW_DISTANCE - BOX_SIZE)
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
typedef enum bool {
    B_TRUE = 1,
    B_FALSE = 0
}bool;

typedef struct param {
    Video video;
    bool start_padding;
    bool end_padding;
    int thread;
}param;

void* smooth_worker(void *args)
{
    param *params = args;
    Video smooth_video = params->video;
    Frame f1, f2;
    struct coord vector_flow;
    float t_i = 0.5; // time factor used for interpolation
    int xb, yb;
    int xf, yf;

    for (int frame = 0; frame < smooth_video.frame_count-2; frame+=2) {
        printf("-- [T%d] processing frame %d/%d\n", params->thread, frame/2 + 1, smooth_video.frame_count/2 );
        f1 = smooth_video.frames[frame];
        f2 = smooth_video.frames[frame+2];
        Image *im = create_copy_image(&smooth_video.frames[frame]);
        for (int x = 0; x < smooth_video.width; ++x) {
            for (int y = 0; y < smooth_video.height; ++y) {
                /*
                 * [xb,yb] = [x,y] - tiVx,y
                 * [xf,yf] = [x,y] + (1 - ti)Vx,y
                 * Fi,x,y = (1 - ti)F0,xb,yb + tiF1,xf,yf
                 */
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
        smooth_video.frames[frame+1] = *im;
        free(im);
    }

    return (void*)params;
}

Video smooth_video(Video video)
{
    printf("Entered %s\n", __func__ );

    Video smooth_video, excerpt;
#ifdef NDEBUG
    int n_threads = 1;
#else
    int n_threads = get_nprocs();
#endif
    pthread_t threads[n_threads];
    int frame_count;

    smooth_video = video;
    smooth_video.fps = video.fps * SMOOTH_FACTOR;
    smooth_video.frame_count = video.frame_count * SMOOTH_FACTOR - 1;
    smooth_video.frames = malloc(smooth_video.frame_count * sizeof(Frame));

    frame_count = smooth_video.frame_count / n_threads;
    excerpt.width = video.width;
    excerpt.height = video.height;
    excerpt.fps = smooth_video.fps;

    memset(smooth_video.frames, 0, smooth_video.frame_count * sizeof(Frame));
    for (int i = smooth_video.frame_count - 1; i >= 0; i -= 2) {
        smooth_video.frames[i] = video.frames[i/2];
    }
    free(video.frames);

    for (int i = 0; i < n_threads; ++i) {
        param *args = malloc(sizeof(param));
        args->start_padding = args->end_padding = 0;
        excerpt.frame_count = frame_count;
        if (i == n_threads - 1)
            excerpt.frame_count += smooth_video.frame_count % n_threads;
        excerpt.frames = &smooth_video.frames[i * frame_count];

        if ((i * frame_count) % 2) {
            args->start_padding = B_TRUE;
            excerpt.frame_count += 1;
            excerpt.frames -= 1;
        }

        if (i != n_threads - 1 && (i * frame_count + frame_count - 1) % 2) {
            args->end_padding = B_TRUE;
            excerpt.frame_count += 1;
        }

        args->video = excerpt;
        args->thread = i + 1;

        pthread_create(&threads[i], NULL, smooth_worker, (void*)args);
        printf("-- Started %s on T%d (frames at Ox%X and of size %d).\n", __func__, i+1, excerpt.frames, excerpt.frame_count);
    }

    for (int i = 0; i < n_threads; ++i) {
        param *args;
        pthread_join(threads[i], (void**)&args);

        printf("-- T%d finished (frames at Ox%X and of size %d).\n", i+1, args->video.frames, args->video.frame_count);
        free(args);
    }

    return smooth_video;
}
