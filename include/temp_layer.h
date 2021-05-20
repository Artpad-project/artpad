#ifndef TEMP_LAYER
#define TEMP_LAYER

#include "queue.h"

typedef struct temp_layer 
{
  int n_y;

  queue *layers_z;
  queue *layerz_y
} temp_layer;

void temp_layer_push(temp_layer* tp, int max, Image *img);
void temp_layer_undo(temp_layer *tp);
void temp_layer_redo(temp_layer *tp);
void temp_layer_update(temp_layer* tp, int max);




#endif
