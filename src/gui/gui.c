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
#include <time.h>
#include "../../include/gui.h"




// Structure of the graphical user interface.



//enum mode {IMAGE_TOOLS = 1,DRAW =2};

//prepare to draw 
void prepare_drawarea(gpointer user_data){
    UserInterface* ui = user_data;
    gtk_fixed_move (ui->drawarea, GTK_WIDGET(ui->area),ui->xpos ,ui->ypos);
    gtk_widget_set_size_request (GTK_WIDGET(ui->area),(gint) ui->im->width, (gint)ui->im->height);
}

	

void draw_total_image(gpointer user_data){
        UserInterface* ui = user_data;
	actualise_image(ui->im,0,0,ui->im->width,ui->im->height);

	free_image(ui->im_zoom);
	ui->im_zoom = rescale_image(ui->im,gtk_adjustment_get_value(ui->zoom_value));

	actualise_image(ui->im_zoom,0,0,ui->im_zoom->width,ui->im_zoom->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im_zoom->pb);

}
void redraw_all(GtkAdjustment *useless,gpointer user_data){
	UserInterface* ui = user_data;
	if(ui->im_zoom)
		draw_total_image(user_data);
}
void color_updated(GtkColorChooser* cc,gpointer user_data){
	UserInterface* ui = user_data;
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->eraser))){
		struct _GdkRGBA* col = malloc(sizeof(struct _GdkRGBA));
		col->red = 0;
		col ->blue = 0;
		col->green = 0;
		col->alpha = 0;
		gtk_color_chooser_get_rgba(cc,col);
		ui->actual_color = pixel_from_GdkRGBA(col);
		//g_print("color : %i,%i,%i,%i\n",ui->actual_color.red,ui->actual_color.green,ui->actual_color.blue,ui->actual_color.alpha);
		gdk_rgba_free(col);
	}
}

// to load a file 
void on_load(GtkFileChooser *fc,gpointer user_data){
    

    UserInterface* ui = user_data;
    if (!ui->currentLayer)
	    add_layer(NULL,user_data);
    ui->currentLayer->im = load_image((char *)gtk_file_chooser_get_filename (fc));    
    merge_from_layers(ui->Layers,ui->im);
    actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
    gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    prepare_drawarea(user_data);
 }


void on_save(GtkFileChooser *fc,gpointer user_data){
    UserInterface* ui = user_data;

    if (ui->im)  
    	save_image(ui->im,NULL,NULL);
}




//ctrl+z function --- POUR LOWEN---
void apply_undo(GtkButton *useless,gpointer user_data){
  UserInterface* ui = user_data;

  temp_layer_undo(ui->currentLayer->tp, &ui->currentLayer->im);
  //g_print("total number: %d\n", ui->currentLayer->tp->n);

  merge_from_layers(ui->Layers, ui->im);
  draw_total_image(user_data);
}

//fonction pour faire ton Redo -- LOWEN ---
void apply_redo(GtkButton *useless,gpointer user_data){
  UserInterface* ui = user_data;

  temp_layer_redo(ui->currentLayer->tp, &ui->currentLayer->im);

  merge_from_layers(ui->Layers, ui->im);
  draw_total_image(user_data);
}

void apply_eraser(GtkRadioButton *useless,gpointer user_data){
	UserInterface* ui = user_data;
	ui->actual_color.alpha = 0;
	if (ui->last_use == ui->fill){

		ui->tolerance = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
		gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->draw_value);

		char* size;
    		int val = asprintf(&size,"size");
		gtk_text_buffer_set_text(ui->drawbuffer,size,val);
	}
	ui->last_use = useless;
}


//changes to pencil mode in draw section
void apply_swap_draw_mode(GtkRadioButton *useless,gpointer user_data){
	UserInterface* ui = user_data;
	if (ui->last_use == ui->eraser)
		color_updated(ui->draw_color, user_data);
	if (ui->last_use == ui->fill){
		ui->tolerance = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
		gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->draw_value);

		char* size;
    		int val = asprintf(&size,"size");

		gtk_text_buffer_set_text(ui->drawbuffer,size,val);
		
	}
   	/*actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);*/  
    	ui->last_use = useless;
}

//changes to flood_fill mode in draw section
void apply_swap_fill_mode(GtkRadioButton *useless,gpointer user_data){
	
    UserInterface* ui = user_data;
	if (ui->last_use == ui->eraser)
		color_updated(ui->draw_color, user_data);
  	if (ui->last_use != ui->fill){
	
	      ui->draw_value = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
	      gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->tolerance);
	      char* size;
	      int val = asprintf(&size,"tolerance");

	      gtk_text_buffer_set_text(ui->drawbuffer,size,val);
	}
	/*actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);*/  
 	ui->last_use = useless;

}



/* Goes back to the original image*/
/*
void see_original(GtkButton *useless,gpointer user_data){
    UserInterface* ui = user_data;

    g_print("Stack state : \n:");
    if (ui->im){
    	UserInterface* ui = user_data;
    	actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
    	gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }

}
*/


// Event handler for the "clicked" signal of the copy button.
void on_start(GtkButton *useless,gpointer user_data)
{
    /*UserInterface *ui = user_data;
    Image* newim = new_image(500,500);
    merge_from_layers(ui->Layers,newim);
    actualise_image(newim,0,0,newim->width,newim->height);
    gtk_image_set_from_pixbuf(ui->area,newim->pb);*/
}

//test on key pressing (actually doesn't work)
/*void on_key_press(GtkWindow oof,GdkEventKey *event,gpointer user_data){
    //UserInterface *ui = user_data;
    g_print("%s\n",((char*)gdk_keyval_name(event->keyval)));
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
    }
}*/

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


void scroll_callback(GtkWidget *useless,GdkEventScroll* event, gpointer user_data){
    UserInterface *ui = user_data;
    int val = gtk_adjustment_get_value(ui->zoom_value);
    if (event->direction  == GDK_SCROLL_DOWN && val > 100)
    	    gtk_adjustment_set_value(ui->zoom_value,val-5);
	   
    if (event->direction == GDK_SCROLL_UP && val < 200)
	    gtk_adjustment_set_value(ui->zoom_value,val+5);
}


//on mouse click detected for drawing (flood_fill)
void mouse_clicked(GtkEventBox* eb,GdkEventButton *event,gpointer user_data){
	UserInterface *ui = user_data;
	int xposi = -ui->xpos + ui->xmouse;
	int yposi = -ui->ypos + ui->ymouse;
    
    if(ui->currentLayer && xposi >= 0  && xposi < ui->currentLayer->im->width && yposi>=0 && yposi < ui->currentLayer->im->height && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->fill)))
        if(event->button == 1 && ui->currentLayer)
        {
            temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);

            struct coord src = {xposi, yposi };
            flood_fill(ui->currentLayer->im,ui->actual_color,src,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
            merge_from_layers(ui->Layers,ui->im);
	    actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
            gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
        }
    
}

//on mouse moved for drawing (pencil)
void mouse_moved(GtkEventBox* eb,GdkEventMotion *event,gpointer user_data){
    UserInterface *ui = user_data;
    
    char *my_string;
    if (ui->currentLayer)
    {
      int xposi = -ui->xpos + event->x;
      int yposi = -ui->ypos + event->y;

      xposi = (int)((float)xposi * (float)100/(gtk_adjustment_get_value(ui->zoom_value)));
      yposi = (int)((float)yposi * (float)100/(gtk_adjustment_get_value(ui->zoom_value)));
      int val = 0;

      if (xposi >= 0  && xposi < ui->im->width && yposi>=0 && yposi < ui->im->height)
        val = asprintf(&my_string,"X: %i,Y: %i",xposi,yposi);
      else 
        val = asprintf(&my_string,"X: -,Y: -");

      if(val <0)
        errx(1,"cannot create the query");
	    
      gtk_text_buffer_set_text(ui->curserpos,my_string,val);
  
      //if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->pencil)))

          if( event->state & GDK_BUTTON1_MASK )
          {
            int pastx = -ui->xpos + ui->xmouse;
            int pasty = -ui->ypos + ui->ymouse;
	    pastx = (int)((float)pastx * (float)100/(gtk_adjustment_get_value(ui->zoom_value)));
	    pasty = (int)((float)pasty * (float)100/(gtk_adjustment_get_value(ui->zoom_value)));
            struct coord src= {pastx,pasty};
            struct coord dest = {xposi,yposi};

            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush1)) ||gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->eraser)))

              pencil(ui->currentLayer->im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));

            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush2)))
              brush(ui->currentLayer->im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));

            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush3)))
                special_brushes(ui->currentLayer->im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
            merge_from_layers(ui->Layers,ui->im);
	    draw_total_image(user_data);
           }     

    if( event->state & GDK_BUTTON2_MASK )
    {
      //g_print("%s\n",my_string);
      ui->xpos += event->x - ui->xmouse;
      ui->ypos += event->y - ui->ymouse;
      gtk_fixed_move (ui->drawarea, GTK_WIDGET(ui->area),ui->xpos ,ui->ypos);
    }
	    
    ui->xmouse = event->x;
    ui->ymouse = event->y;
  }
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
    if (gtk_builder_add_from_file(builder, "src/gui/prototype2.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main"));
    GtkFileChooser* loader =  GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "loader"));  
    GtkButton* saver =  GTK_BUTTON(gtk_builder_get_object(builder, "Save"));  

      
    GtkTextBuffer* curser_position = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cursor_pos"));
    GtkAdjustment* print_width_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "width_value"));  
    GtkAdjustment* print_height_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "height_value"));  
    
    GtkAdjustment* draw_size =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "draw_size"));  

    GtkButton* UNDO_button = GTK_BUTTON(gtk_builder_get_object(builder, "Undo"));
    GtkButton* Redo_button = GTK_BUTTON(gtk_builder_get_object(builder, "Redo"));

    GtkAdjustment* zoom_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "zoom_value"));  

//------------------------------- LAYERS -------------------------------------//
    GtkListBox * layers = GTK_LIST_BOX(gtk_builder_get_object(builder,"Layers"));
    GtkButton* add_layer_button = GTK_BUTTON(gtk_builder_get_object(builder, "add_layer"));
    GtkButton* show_all_layers_button = GTK_BUTTON(gtk_builder_get_object(builder, "show_all_layers"));
    GtkButton* hide_all_layers_button = GTK_BUTTON(gtk_builder_get_object(builder, "hide_all_layers"));


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
    //GtkButton* print_ori_button = GTK_BUTTON(gtk_builder_get_object(builder, "Debug_im2"));

 
// ------------------------------ DRAW BUTTONS---------------------------------//
    GtkColorChooser* draw_color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "Colorconar"));
    GtkTextBuffer* drawbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "buffer_draw"));

    //GtkRadioButton* pencil = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "pencil"));
    GtkRadioButton* flood_fill = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "fill"));
    GtkRadioButton* eraser = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "eraser"));

    GtkRadioButton* brush1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "brush1"));
    GtkRadioButton* brush2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "brush2"));
    GtkRadioButton* brush3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "brush3"));


    struct Pixel pixel = {0,0,0,255};

    UserInterface ui =
    {
      .window = window,
      .drawarea = drawarea,
      .layers = layers,
      .eb_draw = eb_draw,
      .zoom_value = zoom_value,

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
        
      //.pencil = pencil,
      .fill = flood_fill,
      .eraser = eraser,
      .brush1 = brush1,
      .brush2 = brush2,
      .brush3 = brush3,
      .last_use = brush1,

      .draw_size = draw_size,
      .draw_value  = 1,
      .tolerance = 1,
      .nblayers = 0,
      .currentLayer = NULL,
      .maxLayers = 10,
    };

    ui.im = new_image(500,500);
    ui.Layers = create_stack();

    // Connects event handlers.
    // Runs the main loop.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(, "configure-event", G_CALLBACK(getdraw_size), &ui);
    //g_signal_connect(G_OBJECT(window), "configure-event",G_CALLBACK(getdraw_size), &ui);
    
    //g_signal_connect(print_width_value, "value_changed", G_CALLBACK(set_new_width), &ui);
    //g_signal_connect(print_height_value, "value_changed" , G_CALLBACK(set_new_height), &ui);
     
    //g_signal_connect(print_ori_button, "clicked", G_CALLBACK(see_original), &ui);
    g_signal_connect(SAT_button,"clicked", G_CALLBACK(apply_saturation), &ui);
    g_signal_connect(CB_auto,"clicked", G_CALLBACK(apply_auto_color_balance), &ui);
    g_signal_connect(BRI_button,"clicked", G_CALLBACK(apply_brightness), &ui);
    g_signal_connect(CON_button,"clicked", G_CALLBACK(apply_brightness), &ui);
    g_signal_connect(ROTRIGHT_button,"clicked", G_CALLBACK(apply_rot_right), &ui);
    g_signal_connect(ROTLEFT_button,"clicked", G_CALLBACK(apply_rot_left), &ui);
    g_signal_connect(FLIPVERT_button,"clicked", G_CALLBACK(apply_flip_vert), &ui);
    g_signal_connect(FLIPHORI_button,"clicked", G_CALLBACK(apply_flip_hori), &ui);
    g_signal_connect(CB_button, "clicked", G_CALLBACK(apply_color_balance), &ui);
    g_signal_connect(ROT_button, "clicked", G_CALLBACK(apply_rotation), &ui);  

    g_signal_connect(zoom_value, "value_changed" , G_CALLBACK(redraw_all), &ui);

    g_signal_connect(zoom_value, "value_changed" , G_CALLBACK(redraw_all), &ui);

    g_signal_connect(eraser,"clicked", G_CALLBACK(apply_eraser), &ui);
    g_signal_connect(brush1,"clicked", G_CALLBACK(apply_swap_draw_mode), &ui);
    g_signal_connect(brush2,"clicked", G_CALLBACK(apply_swap_draw_mode), &ui);
    g_signal_connect(brush3,"clicked", G_CALLBACK(apply_swap_draw_mode), &ui);

    g_signal_connect(start_button,"clicked", G_CALLBACK(on_start), &ui);
    g_signal_connect(flood_fill,"clicked", G_CALLBACK(apply_swap_fill_mode), &ui);

    g_signal_connect(UNDO_button,"clicked", G_CALLBACK(apply_undo), &ui);
    g_signal_connect(Redo_button,"clicked", G_CALLBACK(apply_redo), &ui);

 

    g_signal_connect(add_layer_button, "clicked", G_CALLBACK(add_layer), &ui);

    g_signal_connect(hide_all_layers_button,"clicked", G_CALLBACK(hide_all_layers), &ui);
    g_signal_connect(show_all_layers_button,"clicked", G_CALLBACK(show_all_layers), &ui);


    g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);
    g_signal_connect(saver, "clicked", G_CALLBACK(on_save), &ui);

    //g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);
    //g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_press), &ui);

    g_signal_connect(eb_draw, "motion-notify-event",G_CALLBACK (mouse_moved), &ui);
    g_signal_connect(eb_draw, "button_press_event",G_CALLBACK (mouse_clicked), &ui);
   
    g_signal_connect(GTK_WIDGET(eb_draw), "scroll_event", G_CALLBACK( scroll_callback ), &ui);
    //
    g_signal_connect(draw_color,"color-set",G_CALLBACK(color_updated),&ui);

    g_signal_connect(layers, "row-activated", G_CALLBACK(set_current_layer), &ui);


    gtk_main();

    if (ui.im){
//todo :  fonction de comparaison d'image
    	free_image(ui.im);
    }
    if (ui.im_zoom){
//todo :  fonction de comparaison d'image
    	free_image(ui.im_zoom);
    }



    //todo :  this free is not working 
    if (!is_stack_empty(ui.Layers)){
    	Stack *tmp = ui.Layers;
    	while(!is_stack_empty(tmp)){
		Layer * cur_layer = pop_from_stack(&tmp);
		//g_print("cur_layer.show : %i\n",cur_layer->show);
		if(cur_layer)
			free_image(cur_layer->im);
		free(cur_layer);

    	}

    }
    free(ui.Layers);
    g_object_unref(builder);
   
    /*
    free_image(im2);
    free(im2);*/

    // Exits.

    return 0;
}
