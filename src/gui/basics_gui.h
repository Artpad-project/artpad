#ifndef BASICS_H_
#define BASICS_H_

#include <stdio.h>
#include <gtk/gtk.h>
#include <err.h>
#include "../image/image.h"


static Image* im ;
Image* im2 ;
Image* im_toprint;


typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkFixed *drawarea;
    GtkFixed *left_zone;
    GtkEventBox* eb_draw;
    GtkDrawingArea* area;           // Drawing area
    GtkButton* start_button;        // Start button
    GtkTextBuffer *curserpos;
    GtkAdjustment *SAT_value;
    GtkAdjustment *CB_value;
    GtkAdjustment *ROT_value;
    GdkRectangle drawzone;
    GtkAdjustment *width_print;
    GtkAdjustment *height_print;
    int width_drawzone;
    int height_drawzone;
    gdouble xmouse;
    gdouble ymouse;

} UserInterface;

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void prepare_drawarea(gpointer user_data);
void on_load(GtkFileChooser *fc,gpointer user_data);

#endif // BASICS_H_