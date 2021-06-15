#ifndef TEMP_LAYER
#define TEMP_LAYER

#include "queue.h"

typedef struct temp_layer 
{
  int n;

  queue *layers_z;
  queue *layers_y
}temp_layer;

void temp_layer_push(temp_layer* tp, int max, Image *img);
void temp_layer_undo(temp_layer *tp, Image *curr_img);
void temp_layer_redo(temp_layer *tp, Image *curr_img);
void temp_layer_update(temp_layer* tp, int max);
void temp_layer_destroy(temp_layer* tp);




#endif
