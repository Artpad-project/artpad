#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include "../../include/video.h"
#include "../../include/stack.h"
#include "../../include/utils.h"
#include "../../include/rotoscopie.h"

#pragma region pixel_checking

#define FRAMES_TO_CHECK 3
#define FILTER_SIZE 3

#define APPLY_PIXEL(_foo) do { \
    _foo(red);                  \
    _foo(green);                \
    _foo(blue);                 \
} while(0)



static int check_pixel(Frame frame, struct coord px, struct Pixel origin)
{
    float red = 0, green = 0, blue = 0;
    int check = 1;
    uint pixel_count= 0;
    int x, y;

#define SUM_PX(_px_color) _px_color += frame.pixels[x][y]._px_color
    for (int i = -FILTER_SIZE/2; i < FILTER_SIZE/2; ++i) {
        x = px.x + i;
        if (x < 0) continue;
        if (x >= frame.width) break;
        for (int j = -FILTER_SIZE/2; j < FILTER_SIZE/2; ++j) {
            y = px.y + j;
            if (y < 0) continue;
            if (y >= frame.height) break;
            APPLY_PIXEL(SUM_PX);
            pixel_count++;
        }
    }

#define MOYENNE_PX(_px_color) _px_color = _px_color / pixel_count
    APPLY_PIXEL(MOYENNE_PX);

#define CHECK(_px_color) check = check && ABS(_px_color - origin._px_color) / 255.0 <= MAGIC_WAND_THRESHOLD
    APPLY_PIXEL(CHECK);

    return check;
}

static int check_pixel_video(Video video, int frame, struct coord px, struct coord origin)
{
    int check = 1;
    for (int i = 0; i < FRAMES_TO_CHECK && check; ++i) {
        if (frame+i >= video.frame_count)
            break;
        check = check_pixel(video.frames[frame+i], px,
                            video.frames[frame+i].pixels[origin.x][origin.y]);
    }

    return check;
}

#pragma endregion pixel_checking

#pragma region threads

typedef struct{
    Video video;
    int x, y;
}thread_args;

void *worker(void *worker_args)
{
    thread_args *args = worker_args;
    int x = args->x, y = args->y;
    Video video = args->video;

    struct coord c, origin = {x, y};
    stack *s = new_stack();
    uint8_t *buf;
    ImageMask mask;

    ASSERT(x >= 0 && y >= 0 && x < video.width && y <video.height, NULL);

    buf = malloc(video.width * video.height * sizeof(uint8_t));

    for (int frame = 0; frame < video.frame_count; ++frame) {

        stack_push(s, origin);
        memset(buf, 0, video.height * video.width * sizeof(uint8_t));
        mask = init_mask(&video.frames[frame]);

        while(!stack_IsEmpty(s))
        {
            c = stack_pop(s);
            x = c.x;
            y = c.y;

            edit_mask(mask, x, y, TRUE);
            buf[y * video.width + x] = 1;

            if (x+1 < video.width && !buf[y * video.width + x + 1] &&
                check_pixel_video(video, frame, (struct coord){x+1,y}, origin))
                stack_push(s, (struct coord){x+1, y});
            if (x-1 >= 0 && !buf[y * video.width + x - 1] &&
                check_pixel_video(video, frame, (struct coord){x-1,y}, origin))
                stack_push(s, (struct coord){x-1, y});
            if (y+1 < video.height && !buf[(y+1)*video.width + x] &&
                check_pixel_video(video, frame, (struct coord){x,y+1}, origin))
                stack_push(s, (struct coord){x, y+1});
            if (y-1 >= 0 && !buf[(y-1)*video.width + x] &&
                check_pixel_video(video, frame, (struct coord){x,y-1}, origin))
                stack_push(s, (struct coord){x, y-1});
        }

        copy_image(mask.mask, &video.frames[frame]);
        free_mask(mask);
    }

    stack_free(s);
    free(buf);

    return (void*)args;
}

#pragma endregion threads

void rotoscopie(Video video, int x, int y) {
    int n_threads = get_nprocs();
    pthread_t threads[n_threads];
    int frame_count = video.frame_count / n_threads;
    Video excerpt;

    excerpt = video;
    excerpt.frame_count = frame_count;

    for (int i = 0; i < n_threads; ++i) {
        thread_args *args = malloc(sizeof(thread_args));
        // Extract part of the video
        if (i == n_threads - 1) excerpt.frame_count += video.frame_count % n_threads;
        excerpt.frames = &video.frames[i*frame_count];

        *args = (thread_args) { excerpt, x, y };

        if (pthread_create(&threads[i], NULL, worker, (void*)args) != 0)
            err(EXIT_FAILURE, "Couldn't create thread.");

        printf("-- Started %s on T%d (frames at Ox%X and of size %d).\n", __func__, i+1, excerpt.frames, excerpt.frame_count);
    }

    for (int i = 0; i < n_threads; ++i) {
        thread_args *args;
        // Get the processed excerpt back
        pthread_join(threads[i], (void**)&args);
        if (!args)
            continue;
        excerpt = args->video;

        printf("-- T%d finished (frames at Ox%X and of size %d -> 0x%X).\n", i+1, excerpt.frames, excerpt.frame_count, &video.frames[i*frame_count]);

        free(args);
    }
}
