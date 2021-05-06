#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "../video/video.h"
#include "../colorimetrie/stack.h"
#include "../utils/utils.h"
#include "rotoscopie.h"

#define FRAMES_TO_CHECK 3

static int check_pixel(struct Pixel px, struct Pixel origin)
{
    return  ((double)ABS(origin.red - px.red) / 255 <= MAGIC_WAND_THRESHOLD) &&
            ((double)ABS(origin.green - px.green) / 255 <= MAGIC_WAND_THRESHOLD) &&
            ((double)ABS(origin.blue - px.blue) / 255 <= MAGIC_WAND_THRESHOLD);
}

static int check_pixel_video(Video video, int frame, struct coord px, struct coord origin)
{
    int check = 1;
    for (int i = FRAMES_TO_CHECK; i < FRAMES_TO_CHECK && check; ++i) {
        if (frame+i >= video.frame_count)
            break;
        check = check &&
                check_pixel(video.frames[frame+i].pixels[px.x][px.y],
                            video.frames[frame+i].pixels[origin.x][origin.y]);
    }

    return check;
}

void rotoscopie(Video video, int x, int y, int *percent)
{
    struct coord c, origin = {x, y};
    stack *s = new_stack();
    uint8_t *buf;
    ImageMask mask;
    int new_percent;

    ASSERT(x >= 0 && y >= 0 && x < video.width && y <video.height);

    buf = malloc(video.width * video.height * sizeof(uint8_t));


    for (int frame = 0; frame < video.frame_count; ++frame) {

        new_percent = (frame*100)/video.frame_count;
        if (new_percent > *percent) {
            *percent = new_percent;
            printf("-- rotoscopie: %d%%\n", *percent);
        }

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

    printf("FINISHED PROCESSING !\n");
    stack_free(s);
    free(buf);
}
