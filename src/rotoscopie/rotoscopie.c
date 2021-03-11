/*!
 *  File created on 3/10/2021 (MM/DD/YYYY) by leo.duboin
 *  Contributors : leo.duboin
 *
 *  File containing all the necessary funtion to perform image extraction from an image.
 *  If possible the task is ran on different threads to make it faster (8 or 4 depending on the number of cores) 
 *  
 *  Added:
 *  - 3/10/2021 : magic_wand, magic_wand_rec
 *  - 3/11/2021 : multithreading using POSIX threads on 4 or 8 threads (magic_wand_async)
 */

#include "rotoscopie.h"

struct args {
    ImageArea mask;
    Pixel origin;
    uint8_t *buf;
    int x, y;
};

typedef struct thread_info {
    int nprocs, nthreads;
    pthread_t *threads;
    struct args args;
}thread_info;

static void * magic_wand_rec(void *arg);
static void magic_wand_async(thread_info t_info);
static int check_pixel(Pixel origin, Pixel pixel);

Image *
magic_wand(Image *im, int x, int y) {
    Image *res;
    ImageArea mask;
    Pixel base;
    uint8_t *buf;
    pthread_t *threads;
    thread_info t_info;
    int nprocs;

    mask = init_area(im);
    base = im->pixels[x][y];
    buf = malloc(im->width * im->height * sizeof(uint8_t));
    nprocs = get_nprocs();

    struct args args = (struct args){mask, base, buf, x, y};

    printf("nprocs: %d", nprocs);

    if (nprocs < 4 || 1)
        magic_wand_rec(&args);
    else {
        nprocs = nprocs >=8 ? 8 : 4;
        threads = malloc(nprocs * sizeof(pthread_t));
        t_info = (thread_info){nprocs, 0, threads, args};
        magic_wand_async(t_info);
        for (int i = 0; i < nprocs; ++i)
            pthread_join(threads[i], NULL);
    }

    res = copy_image(mask.mask);
    free(buf);

    return res;
}

static void *
magic_wand_rec(void *arg) {
    Pixel pixel;
    struct args args = *(struct args *)arg;

    ImageArea mask = args.mask;
    int x = args.x, y = args.y;

    // Check for valid coordinates and infinite reccursion
    if (x < 0 || y < 0 || x >= mask.width || y >= mask.height)
        return (void *)EXIT_FAILURE;
    if (args.buf[y * mask.width + x])
        return (void *)EXIT_FAILURE;

    args.buf[y*mask.width+x] = 1;
    pixel = mask.image->pixels[x][y];

    // Add pixel to mask and reccursively launch the verification to its neighbours
    if (check_pixel(args.origin, pixel)) {
        edit_area(mask, x, y, FALSE);
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (x != 0 && y != 0) {
                    args.x = x + i;
                    args.y = y + j;
                    magic_wand_rec((void *)&args);
                }
            }
        }
    }

    return (void *)EXIT_SUCCESS;
}

static int
check_pixel(Pixel origin, Pixel pixel) {
    int sum_origin = origin.red + origin.green + origin.blue;
    int sum_pixel = pixel.red + pixel.green + pixel.blue;

    return (sum_pixel / sum_origin > 1.0 - MAGIC_WAND_THRESHOLD);
}

/*
 * To make it faster we can execute on different threads. 
 * The number of threads depends on the actual number of cores of the CPU and can be either 4 or 8. 
 * We check the eight pixels surrounding the starting point and create a thread for each of them (or 2 by 2 if we are only using 4 cores).
 */
static void
magic_wand_async(thread_info t_info){
    struct args args;
    int nb_thread;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            if (x != 0 || y != 0) {
                args.x += x;
                args.y += y;
                nb_thread = t_info.nthreads % t_info.nprocs;
                pthread_create(&t_info.threads[nb_thread], NULL, magic_wand_rec, (void *)&args);
                t_info.nthreads += 1;
            }
        }
    }
}
