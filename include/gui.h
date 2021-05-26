//
// Created by leo.duboin on 3/24/21.
//

#ifndef GUI_H
#define GUI_H


#define _GNU_SOURCE
#include <stdio.h>
#include <gtk/gtk.h>

#include "image_scaling.h"
#include "PuissanceN.h"
#include "BalanceAuto.h"
#include "Saturation.h"
#include "Rotation.h"
#include "colorimetrie.h"
#include "stack.h"
#include "ContrastSimple.h"
#include "Flip.h"
#include "Layers.h"

typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkFixed *drawarea;
    GtkListBox *layers;

    GtkEventBox* eb_draw;

    GtkImage* area;           // Drawing area
    GtkButton* start_button;        // Start button
    GtkTextBuffer *curserpos;
    GtkTextBuffer *drawbuffer;
    GtkAdjustment *SAT_value;
    GtkAdjustment *CB_value;
    GtkAdjustment *ROT_value;
    GtkAdjustment *BRI_value;
    GtkAdjustment *CON_value;



    GdkRectangle drawzone;
    GtkAdjustment *width_print;
    GtkAdjustment *height_print;

    
    int xpos;
    int ypos;
    gdouble xmouse;
    gdouble ymouse;

    GtkColorChooser* draw_color;
    struct Pixel actual_color;
    //GtkRadioButton* pencil;
    GtkRadioButton* fill;
    GtkRadioButton* brush1;
    GtkRadioButton* brush2;
    GtkRadioButton* brush3;



    GtkAdjustment *draw_size;
    double draw_value;
    double tolerance;
    int nblayers;
    Layer * currentLayer;
    GtkListBoxRow *r1;
    GtkListBoxRow *r2;

    Image* im ;
    Image* sauv_im1;
    Stack * Layers;


} UserInterface;

//gui tools
void prepare_drawarea(gpointer user_data);

//image tools
void apply_auto_color_balance(GtkButton *button,gpointer user_data);
void apply_color_balance(GtkButton *button,gpointer user_data);
void apply_saturation(GtkButton *button,gpointer user_data);
void apply_brightness(GtkButton *button,gpointer user_data);
void apply_rotation(GtkButton *button,gpointer user_data);
void apply_rot_right(GtkButton *button,gpointer user_data);
void apply_rot_left(GtkButton *button,gpointer user_data);
void apply_flip_hori(GtkButton *button,gpointer user_data);
void apply_flip_vert(GtkButton *button,gpointer user_data);

//layer fcts

void merge_from_layers(Stack* Layers, Image* im);

void show_hide_layer(GtkButton *button,gpointer user_data);
void up_layer(GtkButton *button,gpointer user_data);
void down_layer(GtkButton *button,gpointer user_data);
void set_current_layer(GtkListBox *box ,GtkListBoxRow *listboxrow,gpointer user_data);
void destroy_layer(GtkButton *button,gpointer user_data);
void add_layer(GtkButton *useless,gpointer user_data);



#endif //GUI_H
