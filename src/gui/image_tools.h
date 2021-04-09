#ifndef IMAGETOOLS_H_
#define IMAGETOOLS_H_

#include "basics_tools.h"
#include "../puissancen/PuissanceN.h"
#include "../saturation/Saturation.h"
#include "../rotation/Rotation.c"

void apply_color_balance(GtkButton *button,gpointer user_data);
void apply_saturation(GtkButton *button,gpointer user_data);
void apply_rotation(GtkButton *button,gpointer user_data);
void see_original(GtkButton *useless,gpointer user_data);

#endif // IMAGETOOLS