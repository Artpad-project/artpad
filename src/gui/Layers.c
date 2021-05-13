/*!
 *  File created on 13/5/2021 by tom aubert
 *  Contributors : tom aubert
 *
 *  File containing all the functions necessary to interact with Layers.
 * 
 */

#include "../../inlude/Layers.h"
#include <libalgo.h>

int test(){
	struct l_list_t *layerlist = l_list_create(sizeof(struct Layer));
	l_list_free(layerlist);
	return 0;
}

