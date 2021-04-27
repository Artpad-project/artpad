#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <gtk/gtk.h>
#include <err.h>

#include "image.h"
#include "PuissanceN.h"
#include "Saturation.h"
#include "Rotation.h"

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
    Image *im;
    Image *im2;
    Image *im_toprint;
} UserInterface;

// MAIN FUNCTION TO CALL OUTSIDE OF THE GUI FILE
int launch_app();

// DRAWING TOOLSyy
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void prepare_drawarea(gpointer user_data);
void on_load(GtkFileChooser *fc,gpointer user_data);


// IMAGE TOOLS
void apply_color_balance(GtkButton *button,gpointer user_data);
void apply_saturation(GtkButton *button,gpointer user_data);
void apply_rotation(GtkButton *button,gpointer user_data);
void see_original(GtkButton *useless,gpointer user_data);


// GUI CALLS
void on_key_press(GdkEventKey *event,gpointer user_data);
void mouse_moved(GtkEventBox *eb,GdkEventMotion *event,gpointer user_data);


#endif // GUI_H_
