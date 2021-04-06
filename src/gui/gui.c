/*!
 *  File created on 3/5/2021 by tom.aubert
 *  Contributors : tom aubert
 *
 *  File containing all the functions necessary to use the interface and the other functions.
 *  
 *  
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <gtk/gtk.h>
#include "../image/image.h"
#include "../image/image_scaling.h"

#include <time.h>

// Structure of the graphical user interface.

Image* im ;
Image* im2 ;

typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkFixed *drawarea;
    GtkFixed *left_zone;
    GtkEventBox* eb_draw;
    GtkDrawingArea* area;           // Drawing area
    GtkButton* start_button;        // Start button
    GtkTextBuffer *curserpos;
    GdkRectangle drawzone;
    GdkRectangle selectzone;
    gdouble xmouse;
    gdouble ymouse;
    int shift_pressed;

} UserInterface;

// Event handler for the "draw" signal of the drawing area.

//Useless tres surement
/*void getdraw_size(gpointer user_data){
    UserInterface *ui = user_data;
    ui-> areawidth = gtk_widget_get_allocated_width(GTK_WIDGET(ui->drawarea));
    ui-> areaheight = gtk_widget_get_allocated_height(GTK_WIDGET(ui->drawarea));
    g_print("%i,%i\n",ui->areawidth, ui->areaheight);

}*/

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
    int totwidth =  gtk_widget_get_allocated_width(GTK_WIDGET(ui->left_zone));
    int totheight = gtk_widget_get_allocated_height(GTK_WIDGET(ui->left_zone));
   

    g_print("%i\n",totheight/2 - im->height/2);
    //gtk_widget_set_size_request (GTK_WIDGET(ui->drawarea),newwidth ,newheight);
    gtk_fixed_move (ui->left_zone, GTK_WIDGET(ui->drawarea),totwidth/2 - im->width/2 , totheight/2 - im->height/2);
    gtk_widget_set_size_request (GTK_WIDGET(ui->area),(gint) im->width, (gint)im->height);
    gtk_widget_set_size_request (GTK_WIDGET(ui->eb_draw),(gint) im->width, (gint)im->height);

}

void on_load(GtkFileChooser *fc,gpointer user_data){
    //g_print("load\n");
    UserInterface* ui = user_data;
    im = load_image((char *)gtk_file_chooser_get_filename (fc));
    
    int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));


   
    ui->drawzone.x = draw_width/2 - im->width/2;
    ui->drawzone.y = draw_height/2 - im->height/2;
    ui->drawzone.width = im->width;
    ui->drawzone.height = im->height;
    prepare_drawarea(user_data);
    gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),
                ui->drawzone.x,ui->drawzone.y,ui->drawzone.width,ui->drawzone.height);   
}




// Event handler for the "clicked" signal of the start button.

void on_start(gpointer user_data)
{
    UserInterface *ui = user_data;
    ui->selectzone.x += 100;
    g_print("%i\n",ui->selectzone.x);

}

void on_key_press(GdkEventKey *event,gpointer user_data){
    UserInterface *ui = user_data;
    if (event->keyval == GDK_KEY_p){
        ui->shift_pressed = 1;
    
    }
    if(event->keyval == GDK_KEY_p){
        GdkRectangle old = ui->drawzone;

        ui->drawzone.x += 50;

        g_print("%i,%i,%i,%i\n", old.x,old.y,old.width,old.height);
        gdk_rectangle_union(&old,&ui->drawzone,&old);
        gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),
                old.x,old.y,old.width,old.height);

    }
}



/*
void scroll_callback(GdkEventScroll* event, gpointer user_data){
    UserInterface *ui = user_data;
     g_print("c'est la merguez\n");

    if (ui->shift_pressed){
        g_print("merguez partie\n");
        im2 = rescale_image(im,150);
        im = im2;
        //prepare_drawarea(user_data);
        //gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,500,500);

    }
   
  }
*/

/*
void mouse_moved(GdkEventMotion *event,gpointer user_data){
    UserInterface *ui = user_data;
    
    char *my_string;
    int val = asprintf(&my_string,"X: %i,Y: %i",(int)event->x,(int)event->y);
    if(val <0)
        errx(1,"cannot create the query");

    // g_print("%s\n", my_string);
    gtk_text_buffer_set_text(ui->curserpos,my_string,val);
    if(event->state & GDK_BUTTON2_MASK ){
        struct timeval actual;
        gettimeofday(&actual,NULL);

     
        GdkRectangle old = ui->drawzone;
        gdouble depx = event->x - ui->xmouse;
        gdouble depy = event->y - ui->ymouse;
        g_print("%f,%f\n",depx,depy);
        ui->drawzone.x += (int)depx;
        ui->drawzone.y += (int)depy;

        //gtk_fixed_move(ui->drawarea,ui->eb_draw,100,100);          
        if ( (actual.tv_sec - ui->lastupdate.tv_sec) * 1000000 + actual.tv_usec - ui->lastupdate.tv_usec > 100000){
            printf("took %lu us\n", (actual.tv_sec - ui->lastupdate.tv_sec) * 1000000 + actual.tv_usec - ui->lastupdate.tv_usec);
            gdk_rectangle_union(&old,&ui->drawzone,&old);
            gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),
                old.x,old.y,old.width,old.height);
    
            ui->lastupdate = actual;

        }
    }
    ui->xmouse =event->x;
    ui->ymouse = event->y;

}*/

int main ()
{

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.

    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "prototype.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widge/image/Makefile
src/image/image.c
src/image/image.h
src/image/main.c ts.

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main"));
    GtkFixed* drawarea = GTK_FIXED(gtk_builder_get_object(builder,"fixed_drawable"));
    GtkFixed* left_zone =  GTK_FIXED(gtk_builder_get_object(builder,"fixed_left_side"));

    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "Draw"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "copy"));
    GtkFileChooser* loader =  GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "loader"));  
    
    GtkEventBox *eb_draw = GTK_EVENT_BOX(gtk_builder_get_object(builder, "pepa_humain"));
    gtk_widget_add_events( GTK_WIDGET(eb_draw), GDK_SCROLL_MASK );   
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_KEY_PRESS_MASK);

    GtkTextBuffer* curser_position = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cursor_pos"));

    UserInterface ui ={
                .window = window,
                .drawarea = drawarea,
                .left_zone = left_zone,
                .eb_draw = eb_draw,
                .area = area,
                .start_button = start_button,
                .curserpos = curser_position,
                .drawzone = {0,0,0,0},
                .selectzone = {0,0,0,0},
                .xmouse = 0,
                .ymouse = 0,
                .shift_pressed = 0,           

    };
    // Connects event handlers.
    // Runs the main loop.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(, "configure-event", G_CALLBACK(getdraw_size), &ui);
    //g_signal_connect(G_OBJECT(window), "configure-event",G_CALLBACK(getdraw_size), &ui);
    g_signal_connect(area, "draw", G_CALLBACK(on_draw), &ui);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &ui);
    g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);
    
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);
    g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_press), &ui);

    //g_signal_connect(eb_draw, "motion-notify-event",G_CALLBACK (mouse_moved), &ui);
    //g_signal_connect(eb_draw, "scroll_event", G_CALLBACK( scroll_callback ), &ui);


    gtk_main();
    
    // Exits.

    return 0;
}
