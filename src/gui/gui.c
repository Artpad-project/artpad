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
#include "../puissancen/PuissanceN.h"
#include "../saturation/Saturation.h"
#include "../rotation/Rotation.c"
#include <time.h>

// Structure of the graphical user interface.

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

// Event handler for the "draw" signal of the drawing area.


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

/* Put the color balance*/
void apply_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (im){
        copy_image(im2,im);
        BalanceAbsolue(im,gtk_adjustment_get_value(ui->CB_value));
        sleep(1);
	int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    	int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
    	gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,draw_width,draw_height);

        im_toprint = im;
    }
}

/* Put the color balance*/
void apply_saturation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im2,im);
       // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        SaturationAbsolue(im,gtk_adjustment_get_value(ui->SAT_value));
        sleep(1);
    	int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    	int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
    	gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,draw_width,draw_height);

        im_toprint = im;
    }
}

void apply_rotation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im2,im);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(im,(float)gtk_adjustment_get_value(ui->ROT_value));
        sleep(1);
    	int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    	int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
    	gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,draw_width,draw_height);

        im_toprint = im;
    }
}



/* Goes back to the original image*/
void see_original(GtkButton *useless,gpointer user_data){
    if (im){
    UserInterface* ui = user_data;
    copy_image(im2,im);
    int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
    gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,draw_width,draw_height);
    }
}


/*
// Event handler for the "clicked" signal of the copy button.
void on_start(gpointer user_data)
{
    UserInterface *ui = user_data;
    ui->selectzone.x += 100;
    g_print("%i\n",ui->selectzone.x);

}*/

//test on key pressing (actually work)
void on_key_press(GdkEventKey *event,gpointer user_data){
    UserInterface *ui = user_data;
   
    if(event->keyval == GDK_KEY_p){
        GdkRectangle old = ui->drawzone;

        ui->drawzone.x += 50;

        //g_print("%i,%i,%i,%i\n", old.x,old.y,old.width,old.height);
        gdk_rectangle_union(&old,&ui->drawzone,&old);
        gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),
                old.x,old.y,old.width,old.height);

    }
}
/*
void set_new_width(GtkAdjustment *buffer,gpointer user_data){
    g_print("change width\n");
    UserInterface *ui = user_data;
    im2 = new_image(gtk_adjustment_get_value(ui->width_print),gtk_adjustment_get_value(ui->height_print));
    
    copy_image(im2,im);
    int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
    int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
    ui->drawzone.x = draw_width/2 - im->width/2;
    ui->drawzone.y = draw_height/2 - im->height/2;
    ui->drawzone.width = im->width;
    ui->drawzone.height = im->height;

    prepare_drawarea(user_data);  
}

void set_new_height(GtkAdjustment *buffer,gpointer user_data){
    g_print("change height\n") ;  
}*/

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
    g_print("%i",(int)event->x);
    //gtk_text_buffer_set_text(ui->curserpos,my_string,val);
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

}
*/
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

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main"));
    GtkFixed* drawarea = GTK_FIXED(gtk_builder_get_object(builder,"fixed_drawable"));
    GtkFixed* left_zone =  GTK_FIXED(gtk_builder_get_object(builder,"fixed_left_side"));

    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "Draw"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "copy"));
    GtkButton* print_ori_button = GTK_BUTTON(gtk_builder_get_object(builder, "Debug_im2"));


    GtkFileChooser* loader =  GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "loader"));  
    
    GtkAdjustment* CB_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "color_balance"));  
    GtkAdjustment* SAT_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "saturation"));  
    GtkAdjustment* ROT_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "rotation"));  

    GtkButton* CB_button = GTK_BUTTON(gtk_builder_get_object(builder, "color_balance_go"));
    GtkButton* SAT_button = GTK_BUTTON(gtk_builder_get_object(builder, "saturation_go"));
    GtkButton* ROT_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotation_go"));
   
    GtkEventBox *eb_draw = GTK_EVENT_BOX(gtk_builder_get_object(builder, "pepa_humain"));
    gtk_widget_add_events( GTK_WIDGET(eb_draw), GDK_SCROLL_MASK );   
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_KEY_PRESS_MASK);

    GtkTextBuffer* curser_position = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cursor_pos"));
    GtkAdjustment* print_width_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "width_value"));  
    GtkAdjustment* print_height_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "height_value"));  




    UserInterface ui ={
                .window = window,
                .drawarea = drawarea,
                .left_zone = left_zone,
                .eb_draw = eb_draw,
                .area = area,
                .start_button = start_button,
                .curserpos = curser_position,
                .SAT_value = SAT_value_cursor,
                .CB_value = CB_value_cursor,
                .ROT_value = ROT_value_cursor,
                .drawzone = {0,0,0,0},
                .width_print = print_width_value,
                .height_print = print_height_value,
                .width_drawzone = gtk_widget_get_allocated_width (GTK_WIDGET(left_zone)),
                .height_drawzone = gtk_widget_get_allocated_height (GTK_WIDGET(left_zone)),
                .xmouse = 0,
                .ymouse = 0,         
    };
    
    // Connects event handlers.
    // Runs the main loop.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(, "configure-event", G_CALLBACK(getdraw_size), &ui);
    //g_signal_connect(G_OBJECT(window), "configure-event",G_CALLBACK(getdraw_size), &ui);
    g_signal_connect(area, "draw", G_CALLBACK(on_draw), &ui);
    //g_signal_connect(print_width_value, "value_changed", G_CALLBACK(set_new_width), &ui);
    //g_signal_connect(print_height_value, "value_changed" , G_CALLBACK(set_new_height), &ui);
     
    g_signal_connect(print_ori_button, "clicked", G_CALLBACK(see_original), &ui);
    g_signal_connect(SAT_button,"clicked", G_CALLBACK(apply_saturation), &ui);
    g_signal_connect(CB_button, "clicked", G_CALLBACK(apply_color_balance), &ui);
    g_signal_connect(ROT_button, "clicked", G_CALLBACK(apply_rotation), &ui);   


    g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);
    
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);
    g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_press), &ui);

    //g_signal_connect(eb_draw, "motion-notify-event",G_CALLBACK (mouse_moved), &ui);
    //g_signal_connect(eb_draw, "scroll_event", G_CALLBACK( scroll_callback ), &ui);

    gtk_main();
    
    // Exits.

    return 0;
}
