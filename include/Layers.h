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
#include <stdlib.h>
#include <err.h>

#define Hide 0
#define Show 1
#define Used 2
/*
typedef struct AllLayers{

}*/

typedef struct Layer{
	Image * image; 
	int pos ;
	int mode;
};

int test();

#endif  //LAYERS_H_
