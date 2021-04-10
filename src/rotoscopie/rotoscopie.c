/*!
 *  File created on 3/10/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  File containing all the necessary function to perform image extraction from an image.
 *  If possible the task is ran on different threads to make it faster (8 or 4 depending on the number of cores) 
 *  
 *  Added:
 *  - 3/10/2021 : magic_wand, magic_wand_rec
 *  - 3/11/2021 : multithreading using POSIX threads on 4 or 8 threads (magic_wand_async)
 *  - 3/23/2021 : iterative version using a stack and line segments
 *
 *  Documentation:
 *  - https://www.losingfight.com/blog/2007/08/28/how-to-implement-a-magic-wand-tool/
 */

#include "../../include/rotoscopie.h"

struct cords {
    int x;
    int y;
};

struct line_segment {
    struct cords start;
    struct cords end;
};

struct args {
    Stack *segment_stack;
    ImageMask mask;
    Pixel origin;
    uint8_t *buf;
    struct cords pixel;
};

typedef struct thread_info {
    int nprocs, nthreads;
    pthread_t *threads;
    struct args args;
}thread_info;

static void * magic_wand_rec(void *arg);
static void magic_wand_async(thread_info t_info);
static int check_pixel(Pixel origin, Pixel pixel);
static void line_search(struct args *args);

ImageMask 
magic_wand(Image *im, int x, int y) {
    ImageMask mask;
    Pixel base;
    uint8_t *buf;
    pthread_t *threads;
    thread_info t_info;
    int nprocs;

    mask = init_mask(im);
    ASSERT(x >= 0 && y >= 0 && x < im->width && y <im->height, mask);

    base = im->pixels[x][y];
    buf = malloc(im->width * im->height * sizeof(uint8_t));
    nprocs = get_nprocs();

    struct args args = (struct args){NULL, mask, base, buf, (struct cords){x, y}};

    // FIXME: empty stack when using different threads
    if (nprocs < 4 || 1) {
        args.segment_stack = create_stack();
        line_search(&args);
        magic_wand_rec((void *)&args);
    }
    else {
        nprocs = nprocs >= 8 ? 8 : 4;
        threads = malloc(nprocs * sizeof(pthread_t));
        t_info = (thread_info){nprocs, 0, threads, args};
        magic_wand_async(t_info);
        for (int i = 0; i < nprocs; ++i)
            pthread_join(threads[i], NULL);
    }

    free(buf);
    return mask;
}

static void *
magic_wand_rec(void *arg) {
    struct args args = *(struct args *)arg;

    ImageMask mask = args.mask;
    int line;
    struct line_segment *current_segment;

    // For each line segment:
    //  - walk the points directly above and below it repeating the line search
    //  - map already encountered pixels to avoid infinite recursion
    while(!is_stack_empty(args.segment_stack)) {
        current_segment = (struct line_segment *)pop_from_stack(&args.segment_stack);
        line = current_segment->start.y;
        for (int x = current_segment->start.x; x <= current_segment->end.x; ++x) {
            if (line > 0) {
                args.pixel.y = line - 1;
                if (check_pixel(args.origin, mask.image->pixels[x][args.pixel.y]))
                    line_search(&args);
            }
            if (line < args.mask.height - 1) {
                args.pixel.y = line + 1;
                if (check_pixel(args.origin, mask.image->pixels[x][args.pixel.y]))
                    line_search(&args);
            }
        }
        free(current_segment);
    }

    return (void *)EXIT_SUCCESS;
}

/*
 * To make it faster we can execute on different threads. 
 * The number of threads depends on the actual number of cores of the CPU and can be either 4 or 8. 
 * We check the eight pixels surrounding the starting point and create a thread for each of them (or 2 by 2 if we are only using 4 cores).
 */
static void
magic_wand_async(thread_info t_info){
    struct args args = t_info.args;
    struct cords tmp = args.pixel;
    int nb_thread;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            if (x != 0 || y != 0) {
                args.segment_stack = create_stack();
                args.pixel.x = tmp.x + x;
                args.pixel.y = tmp.y + y;
                line_search(&args);
                nb_thread = t_info.nthreads % t_info.nprocs;
                pthread_create(&t_info.threads[nb_thread], NULL, magic_wand_rec, (void *)&args);
                t_info.nthreads += 1;
            }
        }
    }
}

#pragma region utils

static int
check_pixel(Pixel origin, Pixel pixel) {
    float sum_origin = (float)(origin.red + origin.green + origin.blue);
    float sum_pixel = (float)(pixel.red + pixel.green + pixel.blue);
    float difference = ABS(sum_origin - sum_pixel) / (3*255);
    int check = difference <= MAGIC_WAND_THRESHOLD;

    return check;
}

/*
 * Search left and right to construct a line segment then push it onto the stack.
 */
void line_search(struct args *args) {
    ASSERT(args->pixel.y >= 0 && args->pixel.y < args->mask.height);
    ASSERT(args->pixel.x >= 0 && args->pixel.x < args->mask.width);

    int line_offset = args->pixel.y * args->mask.width;
    if (args->buf[line_offset + args->pixel.x])
        return;

    struct line_segment *segment = malloc(sizeof (struct line_segment));
    *segment = (struct line_segment){args->pixel, args->pixel};

    segment->end.x += 1;

    while (segment->start.x >= -1) {
        if (!segment->start.x ||
            args->buf[line_offset + segment->start.x] ||
            !check_pixel(args->origin, args->mask.image->pixels[segment->start.x][segment->start.y]))
        {
            segment->start.x += 1;
            break;
        }
        args->buf[line_offset + segment->start.x] = 1;
        edit_mask(args->mask, segment->start.x, segment->start.y, TRUE);
        segment->start.x -= 1;
    }
    while (segment->end.x <= args->mask.width) {
        if (segment->end.x == args->mask.width ||
            args->buf[line_offset + segment->end.x] ||
            !check_pixel(args->origin, args->mask.image->pixels[segment->end.x][segment->end.y]))
        {
            segment->end.x -= 1;
            break;
        }
        args->buf[line_offset + segment->end.x] = 1;
        edit_mask(args->mask, segment->end.x, segment->end.y, TRUE);
        segment->end.x += 1;
    }

    if (segment->end.x != segment->start.x)
        args->segment_stack = push_to_stack(args->segment_stack, (void *)segment);
}

#pragma endregion utils
