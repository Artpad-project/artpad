/*!
 *  File created on 9/4/2021 by tom.aubert
 *  Contributors : tom aubert
 *
 *  File containing all the basic functions in the UI.
 */

#include "basics_gui.h"

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    // Gets the 'Game' structure.

    UserInterface *ui = user_data;
    // Sets the background to white.

    // cairo_set_source_rgb(cr, 1, 1, 1);
    // cairo_paint(cr);

    if (im){
        Pixel pixel;
        //int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
        //int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
        float r, g, b;

        for (int x = 0; x < im->width; x++) {
            for (int y = 0; y < im->height; y++) {
                pixel = im->pixels[x][y];
                r = (float) pixel.red / 255;
                g = (float) pixel.green / 255;
                b = (float) pixel.blue / 255;

                cairo_set_source_rgb(cr, r, g, b);
                cairo_rectangle(cr, x, y,1,1);
                cairo_fill(cr);
                //cairo_push_group(cr);
            }
        }
    }

    // Propagates the signal.
    return FALSE;
}

void prepare_drawarea(gpointer user_data){
    UserInterface* ui = user_data;

    int totwidth = im->width > ui->width_drawzone ? im->width : ui->width_drawzone;
    int totheight = im->height>ui->height_drawzone ? im->height : ui->height_drawzone;

    int newwidth = 2*totwidth - im->width;
    int newheight = 2*totheight - im->height;
    gtk_widget_set_size_request (GTK_WIDGET(ui->drawarea),newwidth ,newheight);
    gtk_fixed_move (ui->left_zone, GTK_WIDGET(ui->drawarea),newwidth/2 - im->width/2 , newheight/2 - im->height/2);
    g_print("%i,%i,%i\n",newheight,im->height,newheight/2 - im->height/2);
    gtk_widget_set_size_request (GTK_WIDGET(ui->area),(gint) im->width, (gint)im->height);
    gtk_widget_set_size_request (GTK_WIDGET(ui->eb_draw),(gint) im->width, (gint)im->height);
}

void on_load(GtkFileChooser *fc,gpointer user_data){
    //g_print("load\n");
    UserInterface* ui = user_data;
    im2 = load_image((char *)gtk_file_chooser_get_filename (fc));
    im = copy_image(im2, NULL);
    im_toprint = im;
    int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));


    ui->drawzone.x = draw_width/2 - im->width/2;
    ui->drawzone.y = draw_height/2 - im->height/2;
    ui->drawzone.width = im->width;
    ui->drawzone.height = im->height;
    prepare_drawarea(user_data);
    gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),
                               0,0,draw_width,draw_height);
}

