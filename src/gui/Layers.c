/*!
 *  File created on 13/5/2021 by tom aubert
 *  Contributors : tom aubert
 *
 *  File containing all the functions necessary to interact with Layers.
 * 
 */

#include "../../include/Layers.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

//length of "_layers00\0"
#define APPEND_LENGTH 10

/* saves all layers in a subdirectory layers
 * the nameplate is "{dir_name}/{img original name}_layers00.{filetype};"
 * @param 
 * img: the original image
 * layers: the stack of layers
 * n: the number of layers in the stack
 */
void export(Image *img, Stack *layers, int n)
{
  char *buffer;
  size_t dir_length = 0;

  //creates directory
  if (!is_stack_empty)
  {
    asprintf(&buffer, "draft_%c", img->file);
    // write permission only
    if (mkdir(buffer, S_IWRITE) == -1) 
      errx(EXIT_FAILURE, "couldn't create a directory\n");

    dir_length = strlen(buffer);
  }

  //saves directory
  while(!is_stack_empty(layers))
  {
    Image *draft_img = pop_from_stack(&layers);
    size_t length = strlen(img->file);

    //+1 accounts for the "/" separation
    char draft_path[length + APPEND_LENGTH + dir_length + 1];
    strncat(draft_path, buffer, dir_length);
    strcat(draft_path, "/");
    strncat(draft_path, img->file, dir_length);

    asprintf(&buffer, "_layers%d", n);
    n--;

    strcat(draft_path, buffer);

    save_image(draft_img, draft_path, img->file_type);
  }
}
