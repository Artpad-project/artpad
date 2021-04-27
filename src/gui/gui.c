/*!
 *  File created on 3/5/2021 by tom.aubert
 *  Contributors : tom aubert
 *
 *  File containing all the functions necessary to use the interface and the other functions.
 *  
 *  
 */

#define _GNU_SOURCE

#include <time.h>
#include "../../include/gui.h"

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
/* TODO
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

/* TODO
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


void mouse_moved(GtkEventBox *eb,GdkEventMotion *event,gpointer user_data){
    UserInterface *ui = user_data;
    
    char *my_string;
    int val = asprintf(&my_string,"X: %i,Y: %i",(int)event->x,(int)event->y);
    if(val <0)
        errx(1,"cannot create the query");
    //g_print("%s",my_string);
    gtk_text_buffer_set_text(ui->curserpos,my_string,val);
}

int launch_app ()
{

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../src/gui/prototype.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    //Divers
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main"));
    GtkFileChooser* loader =  GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "loader"));
    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "Draw"));
    GtkEventBox *eb_draw = GTK_EVENT_BOX(gtk_builder_get_object(builder, "pepa_humain"));

    //Fixeds
    GtkFixed* drawarea = GTK_FIXED(gtk_builder_get_object(builder,"fixed_drawable"));
    GtkFixed* left_zone =  GTK_FIXED(gtk_builder_get_object(builder,"fixed_left_side"));

    //Buttons_Basics
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "copy"));
    GtkButton* print_ori_button = GTK_BUTTON(gtk_builder_get_object(builder, "Debug_im2"));

    //Buttons Treatment
    GtkButton* CB_button = GTK_BUTTON(gtk_builder_get_object(builder, "color_balance_go"));
    GtkButton* SAT_button = GTK_BUTTON(gtk_builder_get_object(builder, "saturation_go"));
    GtkButton* ROT_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotation_go"));

    //Buttons Draw

    //Adjustments
    GtkAdjustment* CB_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "color_balance"));  
    GtkAdjustment* SAT_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "saturation"));  
    GtkAdjustment* ROT_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "rotation"));
    GtkAdjustment* print_width_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "width_value"));
    GtkAdjustment* print_height_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "height_value"));

    //TextBuffers
    GtkTextBuffer* curser_position = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cursor_pos"));

    //adding events
    gtk_widget_add_events( GTK_WIDGET(eb_draw), GDK_SCROLL_MASK );   
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_KEY_PRESS_MASK);


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
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(area, "draw", G_CALLBACK(on_draw), &ui);
    //g_signal_connect(print_width_value, "value_changed", G_CALLBACK(set_new_width), &ui);
    //g_signal_connect(print_height_value, "value_changed" , G_CALLBACK(set_new_height), &ui);

    //File selection
    g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);

    //Image_Treatment activation
    g_signal_connect(print_ori_button, "clicked", G_CALLBACK(see_original), &ui);
    g_signal_connect(SAT_button,"clicked", G_CALLBACK(apply_saturation), &ui);
    g_signal_connect(CB_button, "clicked", G_CALLBACK(apply_color_balance), &ui);
    g_signal_connect(ROT_button, "clicked", G_CALLBACK(apply_rotation), &ui);

    //Key events
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);
    g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_press), &ui);

    //mouse events
    g_signal_connect(eb_draw, "motion-notify-event",G_CALLBACK (mouse_moved), &ui);
    //g_signal_connect(eb_draw, "scroll_event", G_CALLBACK( scroll_callback ), &ui);

    gtk_main();
    
    // Exits.

    return 0;
}
