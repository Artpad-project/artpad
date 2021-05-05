
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
#include "../balanceauto/BalanceAuto.h"

#include "../saturation/Saturation.h"
#include "../rotation/Rotation.c"
#include "../colorimetrie/colorimetrie.h"
#include "../colorimetrie/stack.h"
#include "../contrastsimple/ContrastSimple.h"
#include "../flip/Flip.h"
#include <time.h>

// Structure of the graphical user interface.

static Image* im ;
Image* im2 ;
Image* sauv_im1;

enum mode {IMAGE_TOOLS = 1,DRAW =2};

typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkFixed *drawarea;
    GtkStack *stack_used;

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
    GtkRadioButton* pencil;
    GtkRadioButton* fill;
    GtkAdjustment *draw_size;
    int draw_value;
    int tolerance;

} UserInterface;

// Event handler for the "draw" signal of the drawing area.

/*
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
}*/

void prepare_drawarea(gpointer user_data){
    UserInterface* ui = user_data;

    
    //gtk_widget_set_size_request (GTK_WIDGET(ui->drawarea),newwidth ,newheight);
    gtk_fixed_move (ui->drawarea, GTK_WIDGET(ui->area),0 ,0);
    
    ui->xpos = 0;
    ui->ypos = 0,

    gtk_widget_set_size_request (GTK_WIDGET(ui->area),(gint) im->width, (gint)im->height);
    

}

void on_load(GtkFileChooser *fc,gpointer user_data){
    UserInterface* ui = user_data;
    im2 = load_image((char *)gtk_file_chooser_get_filename (fc));
    sauv_im1 =  load_image((char *)gtk_file_chooser_get_filename (fc));
    im = copy_image(im2, NULL);
        
 
    prepare_drawarea(user_data);
    actualise_image(im,0,0,im->width,im->height);
    gtk_image_set_from_pixbuf(ui->area,im->pb);
}


/* Put the color balance*/
void apply_auto_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (im){
        copy_image(im,sauv_im1);
        copy_image(im2,im);
        BalanceAuto(im);
    	actualise_image(im,0,0,im->width,im->height);
        gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}



/* Put the color balance*/
void apply_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (im){
        copy_image(im,sauv_im1);
        copy_image(im2,im);
        BalanceAbsolue(im,gtk_adjustment_get_value(ui->CB_value));
    	actualise_image(im,0,0,im->width,im->height);
        gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

/* Put the color balance*/
void apply_saturation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        copy_image(im2,im);
       // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        SaturationAbsolue(im,gtk_adjustment_get_value(ui->SAT_value));
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}


void apply_brightness(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        copy_image(im2,im);
       // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Contrast(im,gtk_adjustment_get_value(ui->CON_value),gtk_adjustment_get_value(ui->BRI_value));
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}



void apply_rotation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        copy_image(im2,im);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(im,(float)gtk_adjustment_get_value(ui->ROT_value));
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

void apply_rot_right(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(im,(float)90);
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

void apply_rot_left(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(im,(float)-90);
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

void apply_flip_hori(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        FlipHorizon(im);
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

void apply_flip_vert(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (im){
        copy_image(im,sauv_im1);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        FlipVertical(im);
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}


void apply_undo(GtkButton *useless,gpointer user_data){
    if (im){
    	UserInterface* ui = user_data;
    	copy_image(sauv_im1,im);
    	actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

void apply_swap_draw_mode(GtkButton *useless,gpointer user_data){
    if (im){
	
    	UserInterface* ui = user_data;
    	g_print("before draw_mode: %i,%i\n",ui->draw_value,ui->tolerance);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->pencil))){
		ui->tolerance = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
		gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->draw_value);
		char* size;
       		int val = asprintf(&size,"size");

		gtk_text_buffer_set_text(ui->drawbuffer,size,val);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->fill))){
		ui->draw_value = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
		gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->tolerance);
		char* size;
		int val = asprintf(&size,"tolerance");
		gtk_text_buffer_set_text(ui->drawbuffer,size,val);

	}


    	g_print("after : %i,%i\n",ui->draw_value,ui->tolerance);

	/*actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);*/
   
    }
}



/* Goes back to the original image*/
void see_original(GtkButton *useless,gpointer user_data){
    if (im){
    	UserInterface* ui = user_data;
    	copy_image(im2,im);
    	/*actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);*/
   
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

//test on key pressing (actually doesn't work)
void on_key_press(GtkWindow oof,GdkEventKey *event,gpointer user_data){
    UserInterface *ui = user_data;
    /*g_print("%s\n",((char*)gdk_keyval_name(event->keyval)));
    switch (event->keyval)
    {
        case GDK_KEY_Z:
            g_print("key pressed: Z\n");
            break;
        case GDK_KEY_z:
            g_print("key pressed: z\n");
            break;
        case GDK_KEY_S:
        case GDK_KEY_s:
            if (event->state & GDK_SHIFT_MASK)
            {
                printf("key pressed: %s\n", "shift + s");
            }
            else if (event->state & GDK_CONTROL_MASK)
            {
                printf("key pressed: %s\n", "ctrl + s");
            }
            else
            {
                printf("key pressed: %s\n", "s");
            }
            break;
    }

    if(strcmp(gdk_keyval_name(event->keyval), "Z") == 0){
        g_print("controle z");
    }*/
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

void mouse_clicked(GtkEventBox* eb,GdkEventButton *event,gpointer user_data){
	UserInterface *ui = user_data;
	int xposi = -ui->xpos + ui->xmouse;
	int yposi = -ui->ypos + ui->ymouse;
    if(strcmp((char*)gtk_stack_get_visible_child_name (ui->stack_used),"page1") == 0){
    	if(im && xposi >= 0  && xposi < im->width && yposi>=0 && yposi < im->height && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->fill)))
		    if(event->button == 1 && im){
			    struct coord src = {xposi, yposi };
			    flood_fill(im,ui->actual_color,src,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
			    actualise_image(im,0,0,im->width,im->height);
	  		    gtk_image_set_from_pixbuf(ui->area,im->pb);
		}
    }
}


void mouse_moved(GtkEventBox* eb,GdkEventMotion *event,gpointer user_data){
    UserInterface *ui = user_data;
    
    char *my_string;
    if (im){
	int xposi = -ui->xpos + event->x;
	int yposi = -ui->ypos + event->y;
	int val = 0;
	if (xposi >= 0  && xposi < im->width && yposi>=0 && yposi < im->height)
		val = asprintf(&my_string,"X: %i,Y: %i",xposi,yposi);
	else {val = asprintf(&my_string,"X: -,Y: -");}
	if(val <0)
		errx(1,"cannot create the query");
	    
	gtk_text_buffer_set_text(ui->curserpos,my_string,val);
	if(strcmp((char*)gtk_stack_get_visible_child_name (ui->stack_used),"page1") == 0){
    
	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->pencil)))
		    if( event->state & GDK_BUTTON1_MASK ){
		         //struct timeval actual;
		        //gettimeofday(&actual,NULL);
		        g_print("%s\n",my_string);
		        int pastx = -ui->xpos + ui->xmouse;
		        int pasty = -ui->ypos + ui->ymouse;
		        struct coord src= {pastx,pasty};
		        struct coord dest = {xposi,yposi};
		        paintLine(im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
		        actualise_image(im,0,0,im->width,im->height);
		        gtk_image_set_from_pixbuf(ui->area,im->pb);
		    }
    }

	if( event->state & GDK_BUTTON2_MASK ){
		g_print("%s\n",my_string);
		ui->xpos += event->x - ui->xmouse;
		ui->ypos += event->y - ui->ymouse;
		gtk_fixed_move (ui->drawarea, GTK_WIDGET(ui->area),ui->xpos ,ui->ypos);
	}
	    
	ui->xmouse = event->x;
	ui->ymouse = event->y;
    }
}

void color_updated(GtkColorChooser* cc,gpointer user_data){
	UserInterface* ui = user_data;

	struct _GdkRGBA* col = malloc(sizeof(struct _GdkRGBA));
	col->red = 0;
	col ->blue = 0;
	col->green = 0;
	col->alpha = 0;
	gtk_color_chooser_get_rgba(cc,col);
	ui->actual_color = pixel_from_GdkRGBA(col);
	g_print("color : %i,%i,%i\n",ui->actual_color.red,ui->actual_color.green,ui->actual_color.blue);
}

int main ()
{

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.

    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "prototype2.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main"));
    GtkFileChooser* loader =  GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "loader"));  
    GtkStack* stack_used = GTK_STACK(gtk_builder_get_object(builder,"stack1"));
      
    GtkTextBuffer* curser_position = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cursor_pos"));
    GtkAdjustment* print_width_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "width_value"));  
    GtkAdjustment* print_height_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "height_value"));  
    
    GtkAdjustment* draw_size =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "draw_size"));  


    GtkButton* UNDO_button = GTK_BUTTON(gtk_builder_get_object(builder, "Undo"));



// ------------------------------ DRAWING ------------------------------------//
    GtkFixed* drawarea = GTK_FIXED(gtk_builder_get_object(builder,"fixed_drawable"));
    GtkImage* area = GTK_IMAGE(gtk_builder_get_object(builder, "Image_evidemment"));

 
    GtkEventBox *eb_draw = GTK_EVENT_BOX(gtk_builder_get_object(builder, "pepa_humain"));
    gtk_widget_add_events( GTK_WIDGET(eb_draw), GDK_SCROLL_MASK );   
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(GTK_WIDGET(eb_draw),GDK_KEY_PRESS_MASK);
    
   
    
    
// ------------------------------ IMAGE TOOLS--------------------------------//
    
    GtkAdjustment* CB_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "color_balance"));  
    GtkAdjustment* SAT_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "saturation"));  
    GtkAdjustment* ROT_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "rotation"));  
    GtkAdjustment* BRI_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "Brightness_adj"));  
    GtkAdjustment* CON_value_cursor =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "Contrast_adj"));  


    GtkButton* CB_button = GTK_BUTTON(gtk_builder_get_object(builder, "color_balance_go"));
    GtkButton* CB_auto = GTK_BUTTON(gtk_builder_get_object(builder, "color_balance_auto"));
    GtkButton* SAT_button = GTK_BUTTON(gtk_builder_get_object(builder, "saturation_go"));
    GtkButton* ROT_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotation_go"));
    GtkButton* BRI_button = GTK_BUTTON(gtk_builder_get_object(builder, "brightness_go"));
    GtkButton* CON_button = GTK_BUTTON(gtk_builder_get_object(builder, "contrast_go"));
    
    GtkButton* FLIPHORI_button = GTK_BUTTON(gtk_builder_get_object(builder, "flip_hori_go"));
    GtkButton* FLIPVERT_button = GTK_BUTTON(gtk_builder_get_object(builder, "flip_vert_go"));
    GtkButton* ROTRIGHT_button = GTK_BUTTON(gtk_builder_get_object(builder, "rot_right_go"));
    GtkButton* ROTLEFT_button = GTK_BUTTON(gtk_builder_get_object(builder, "rot_left_go"));

    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "copy"));
    GtkButton* print_ori_button = GTK_BUTTON(gtk_builder_get_object(builder, "Debug_im2"));

 
// ------------------------------ DRAW BUTTONS---------------------------------//
    GtkColorChooser* draw_color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "Colorconar"));
    GtkTextBuffer* drawbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "buffer_draw"));

    GtkRadioButton* pencil = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "pencil"));
    GtkRadioButton* flood_fill = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "fill"));

    struct _GdkRGBA *col = malloc(sizeof(struct _GdkRGBA));
    col->red = 0;
    col->blue = 0;
    col->green = 0;
    col->alpha = 255;
    gtk_color_chooser_set_rgba(draw_color,col);
    struct Pixel pixel = {0,0,0,255};
    //im = new_image(1650,900);
    //copy_image(im2,im);

   

    

    UserInterface ui ={
                .window = window,
                .drawarea = drawarea,
                .stack_used = stack_used,

                .eb_draw = eb_draw,

                .area = area,
                .start_button = start_button,
                .curserpos = curser_position,
		.drawbuffer = drawbuffer,

                .SAT_value = SAT_value_cursor,
                .CB_value = CB_value_cursor,
                .ROT_value = ROT_value_cursor,
                .BRI_value = BRI_value_cursor,
                .CON_value = CON_value_cursor,
                .drawzone = {0,0,0,0},
                .width_print = print_width_value,
                .height_print = print_height_value,
		.xpos = 0,
	        .ypos = 0,
                .xmouse = 0,
                .ymouse = 0,

       		.draw_color = draw_color,
	        .actual_color = pixel,

		.pencil = pencil,
		.fill = flood_fill,
		.draw_size = draw_size,
		.draw_value  = 1,
		.tolerance = 1
    };
    
    // Connects event handlers.
    // Runs the main loop.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(, "configure-event", G_CALLBACK(getdraw_size), &ui);
    //g_signal_connect(G_OBJECT(window), "configure-event",G_CALLBACK(getdraw_size), &ui);
    
    //g_signal_connect(print_width_value, "value_changed", G_CALLBACK(set_new_width), &ui);
    //g_signal_connect(print_height_value, "value_changed" , G_CALLBACK(set_new_height), &ui);
     
    g_signal_connect(print_ori_button, "clicked", G_CALLBACK(see_original), &ui);
    g_signal_connect(SAT_button,"clicked", G_CALLBACK(apply_saturation), &ui);
    g_signal_connect(CB_auto,"clicked", G_CALLBACK(apply_auto_color_balance), &ui);
    g_signal_connect(BRI_button,"clicked", G_CALLBACK(apply_brightness), &ui);
    g_signal_connect(CON_button,"clicked", G_CALLBACK(apply_brightness), &ui);
    g_signal_connect(ROTRIGHT_button,"clicked", G_CALLBACK(apply_rot_right), &ui);
    g_signal_connect(ROTLEFT_button,"clicked", G_CALLBACK(apply_rot_left), &ui);
    g_signal_connect(FLIPVERT_button,"clicked", G_CALLBACK(apply_flip_vert), &ui);
    g_signal_connect(FLIPHORI_button,"clicked", G_CALLBACK(apply_flip_hori), &ui);


    g_signal_connect(pencil,"clicked", G_CALLBACK(apply_swap_draw_mode), &ui);
    //g_signal_connect(flood_fill,"clicked", G_CALLBACK(apply_flood_fill_mode), &ui);

    g_signal_connect(UNDO_button,"clicked", G_CALLBACK(apply_undo), &ui);


    g_signal_connect(CB_button, "clicked", G_CALLBACK(apply_color_balance), &ui);
    g_signal_connect(ROT_button, "clicked", G_CALLBACK(apply_rotation), &ui);   


    g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);
    
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);
    //g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_press), &ui);

    g_signal_connect(eb_draw, "motion-notify-event",G_CALLBACK (mouse_moved), &ui);
    g_signal_connect(eb_draw, "button_press_event",G_CALLBACK (mouse_clicked), &ui);

    //g_signal_connect(eb_draw, "scroll_event", G_CALLBACK( scroll_callback ), &ui);
    //
    g_signal_connect(draw_color,"color-set",G_CALLBACK(color_updated),&ui);

    gtk_main();
    
    // Exits.

    return 0;
}
