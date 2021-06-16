/*!
 *  File created on 13/5/2021 by tom.aubert
 *  Contributors : tom.aubert
 *
 *  File containing all the necessary functions prototypes for layers.
 *
 */

#ifndef LAYERS_H_
#define LAYERS_H_

#include <gtk/gtk.h>
#include "image.h"
#include "utils.h"
#include <stdlib.h>
#include <err.h>

#define Hide 0
#define Show 1
#define Used 2
/*
typedef struct AllLayers{

}*/


typedef struct Layer 
{
    struct Image *im;
    int show;
    int relativxpos;
    int relativypos;
    GtkListBoxRow * lbr;
    GtkButton * button;
}Layer;

int get_index_layer(Stack* Layers,GtkListBoxRow * lbr);
void apply_to_all_layers(void (*function)(void*,void*),void* arg1,void* arg2,Stack * Layers);
void export(Image *img, Stack *layers, int n);

#endif  //LAYERS_H_
