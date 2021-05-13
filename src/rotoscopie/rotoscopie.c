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

#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "../../include/image.h"
#include "../../include/stack.h"
#include "../../include/utils.h"
#include "../../include/rotoscopie.h"

static int check_pixel(struct Pixel px, struct Pixel origin)
{
    return  ((double)ABS(origin.red - px.red) / 255 <= MAGIC_WAND_THRESHOLD) &&
            ((double)ABS(origin.green - px.green) / 255 <= MAGIC_WAND_THRESHOLD) &&
            ((double)ABS(origin.blue - px.blue) / 255 <= MAGIC_WAND_THRESHOLD);
}

ImageMask magic_wand(struct Image *img, int x, int y)
{
    struct coord c = {x, y};
    stack *s = new_stack();
    ImageMask mask = init_mask(img);
    uint8_t *buf;
    Pixel px;

    ASSERT(x >= 0 && y >= 0 && x < img->width && y <img->height, mask);

    stack_push(s, c);
    buf = malloc(img->width * img->height * sizeof(uint8_t));
    memset(buf, 0, img->height * img->width * sizeof(uint8_t));

    while(!stack_IsEmpty(s))
    {
        c = stack_pop(s);
        x = c.x;
        y = c.y;

        edit_mask(mask, x, y, TRUE);
        buf[y * img->width + x] = 1;
        px = img->pixels[x][y];

        if (x+1 < img->width && !buf[y * img->width + x + 1] &&
                check_pixel(img->pixels[x + 1][y], px))
            stack_push(s, (struct coord){x+1, y});
        if (x-1 >= 0 && !buf[y * img->width + x - 1] &&
                check_pixel(img->pixels[x - 1][y], px))
            stack_push(s, (struct coord){x-1, y});
        if (y+1 < img->height && !buf[(y+1)*img->width + x] &&
                check_pixel(img->pixels[x][y + 1], px))
            stack_push(s, (struct coord){x, y+1});
        if (y-1 >= 0 && !buf[(y-1)*img->width + x] &&
                check_pixel(img->pixels[x][y - 1], px))
            stack_push(s, (struct coord){x, y-1});
    }

    stack_free(s);
    free(buf);

    return mask;
}
