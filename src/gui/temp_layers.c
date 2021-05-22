#include "../../include/temp_layer.h"
#include "../../include/image.h"

//pushes a new value into the queue while updating number of elements
void temp_layer_push(temp_layer* tp, int max, Image img)
{
  if (tp->n+1 == max)
  {
    Image *temp;
    tp->layers_z = queue_pop(queue_push(tp->layers_z, img), temp);
    free_image(temp);
  }

  else
  {
    tp->layers_z = queue_push(tp->layers_z, img);
    tp->n += 1;
  }

  queue_empty(&tp->layers_y);
}

// ctrl+z will pop the last element and add current image to layer_y
void temp_layer_undo(temp_layer *tp, Image *curr_img)
{
  if (tp->layers_z == NULL)
    return;

  Image *new_img = new_image(curr_img->width, curr_img->height);
  new_img = copy_image(curr_img, new_img);
  tp->layers_y = queue_push(tp->layers_y, *new_img);

  tp->layers_z = queue_pop(tp->layers_z, curr_img);
}

// ctrl+y will simply pop layers_y and add the current image back into layers_z
void temp_layer_redo(temp_layer *tp, Image *curr_img)
{
  if (tp->layers_y == NULL)
    return;

  Image *new_img = new_image(curr_img->width, curr_img->height);
  new_img = copy_image(curr_img, new_img);
  tp->layers_z = queue_push(tp->layers_z, *new_img);

  tp->layers_y = queue_pop(tp->layers_y, curr_img);
}

//updates temp_layer accordingly when 
//user updates number of undo and redo max
void temp_layer_update(temp_layer* tp, int max)
{
  while (tp->n > max)
  {
    Image *temp;
    tp->layers_z = queue_pop(tp->layers_z, temp);
    free_image(temp);
    tp->n -= 1;
  }
}
