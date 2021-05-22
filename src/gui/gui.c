
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


#include "../../include/image_scaling.h"
#include "../../include/PuissanceN.h"
#include "../../include/BalanceAuto.h"
#include "../../include/Saturation.h"
#include "../../include/Rotation.h"
#include "../../include/colorimetrie.h"
#include "../../include/stack.h"
#include "../../include/ContrastSimple.h"
#include "../../include/Flip.h"



// Structure of the graphical user interface.
//a
void add_layer(GtkButton *useless,gpointer user_data);
void set_current_layer(GtkListBox *box ,GtkListBoxRow *listboxrow,gpointer user_data);


static Image* im ;
Image* sauv_im1;
Stack * Layers;

typedef struct Layer 
{
    struct Image *im;
    int show;

    int relativxpos;
    int relativypos;
    GtkListBoxRow * lbr;
}Layer;

Layer * current_layer;

int get_index_layer(Stack* Layers,GtkListBoxRow * lbr){
	Stack * tmp = Layers;
	int pos = 0;
	if (!is_stack_empty(tmp)){
		g_print("whaouh\n");
		while (tmp->next != NULL ){
			Layer * act = tmp->data;
			if (act->lbr == lbr)			
				break;

			pos ++;
			tmp = tmp->next;
		}

	}
	return pos;
}




enum mode {IMAGE_TOOLS = 1,DRAW =2};

typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkFixed *drawarea;
    GtkStack *stack_used;
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
    GtkRadioButton* pencil;
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

} UserInterface;


void prepare_drawarea(gpointer user_data){
    UserInterface* ui = user_data;

    
    //gtk_widget_set_size_request (GTK_WIDGET(ui->drawarea),newwidth ,newheight);
    gtk_fixed_move (ui->drawarea, GTK_WIDGET(ui->area),ui->xpos ,ui->ypos);
    gtk_widget_set_size_request (GTK_WIDGET(ui->area),(gint) im->width, (gint)im->height);
    

}

void on_load(GtkFileChooser *fc,gpointer user_data){
    

    UserInterface* ui = user_data;
    if (!ui->currentLayer){
	    add_layer(NULL,user_data);
    }

    
    ui->currentLayer->im = load_image((char *)gtk_file_chooser_get_filename (fc));
    sauv_im1 =  load_image((char *)gtk_file_chooser_get_filename (fc));
    im = ui->currentLayer->im;
    actualise_image(im,0,0,im->width,im->height);
    gtk_image_set_from_pixbuf(ui->area,im->pb);
    prepare_drawarea(user_data);
 }


void on_save(GtkFileChooser *fc,gpointer user_data){
    if (im)  
    	save_image(im,NULL,NULL);

}


/* Put the color balance*/
void apply_auto_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (im){
        sauv_im1 = copy_image(im,sauv_im1);
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
        sauv_im1 = copy_image(im,sauv_im1);

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
        sauv_im1 = copy_image(im,sauv_im1);
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
        sauv_im1 = copy_image(im,sauv_im1);
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
        sauv_im1 = copy_image(im,sauv_im1);
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(im,(float)gtk_adjustment_get_value(ui->ROT_value));
        actualise_image(im,0,0,im->width,im->height);
	gtk_image_set_from_pixbuf(ui->area,im->pb);
    }
}

void apply_rot_right(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    g_print("nblayers = %i\n",ui->nblayers);

    //free_image(im);
    if (im){
        sauv_im1 = copy_image(im,sauv_im1);
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
        sauv_im1 = copy_image(im,sauv_im1);
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
        sauv_im1 = copy_image(im,sauv_im1);
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
        sauv_im1 = copy_image(im,sauv_im1);
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
    	UserInterface* ui = user_data;


	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->pencil))){
		ui->tolerance = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
		gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->draw_value);
		char* size;
       		int val = asprintf(&size,"size");

		gtk_text_buffer_set_text(ui->drawbuffer,size,val);
	}
   	/*actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);*/  
    
}

void apply_swap_fill_mode(GtkButton *useless,gpointer user_data){
	
    	UserInterface* ui = user_data;
  

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->fill))){
		

		ui->draw_value = gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size));
		gtk_adjustment_set_value (GTK_ADJUSTMENT(ui->draw_size),ui->tolerance);
		char* size;
		int val = asprintf(&size,"tolerance");
		gtk_text_buffer_set_text(ui->drawbuffer,size,val);

	}
	/*actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);*/  
}



/* Goes back to the original image*/
void see_original(GtkButton *useless,gpointer user_data){
    g_print("Stack state : \n:");
    if (im){
    	UserInterface* ui = user_data;
    	actualise_image(im,0,0,im->width,im->height);
    	gtk_image_set_from_pixbuf(ui->area,im->pb);
   
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
		        //g_print("%s\n",my_string);
		        int pastx = -ui->xpos + ui->xmouse;
		        int pasty = -ui->ypos + ui->ymouse;
		        struct coord src= {pastx,pasty};
		        struct coord dest = {xposi,yposi};
			if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush1)))
				pencil(im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));

			if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush2)))
				brush(im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));

			if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush3)))
		        	special_brushes(im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
		        actualise_image(im,0,0,im->width,im->height);
		        gtk_image_set_from_pixbuf(ui->area,im->pb);
		    }

      /*
	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->brush)))
		    if( event->state & GDK_BUTTON1_MASK ){
		         //struct timeval actual;
		        //gettimeofday(&actual,NULL);
		        g_print("%s\n",my_string);
		        int pastx = -ui->xpos + ui->xmouse;
		        int pasty = -ui->ypos + ui->ymouse;
		        struct coord src= {pastx,pasty};
		        struct coord dest = {xposi,yposi};
		        brush(im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
		        actualise_image(im,0,0,im->width,im->height);
		        gtk_image_set_from_pixbuf(ui->area,im->pb);
		    }

	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->specialBrush)))
		    if( event->state & GDK_BUTTON1_MASK ){
		         //struct timeval actual;
		        //gettimeofday(&actual,NULL);
		        g_print("%s\n",my_string);
		        int pastx = -ui->xpos + ui->xmouse;
		        int pasty = -ui->ypos + ui->ymouse;
		        struct coord src= {pastx,pasty};
		        struct coord dest = {xposi,yposi};
		        pencil(im,ui->actual_color,src,dest,gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->draw_size)));
		        actualise_image(im,0,0,im->width,im->height);
		        gtk_image_set_from_pixbuf(ui->area,im->pb);
		    }
        */
    }

	if( event->state & GDK_BUTTON2_MASK ){
		//g_print("%s\n",my_string);
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

void show_hide_layer(GtkButton *button,gpointer user_data){
    GtkListBoxRow *lbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button))));
    GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(lbr)));
    if(gtk_list_box_row_is_selected (lbr))
         gtk_list_box_unselect_row (lb,lbr);
    else
         gtk_list_box_select_row (lb,lbr);

    g_print("%i\n",gtk_list_box_row_get_index (lbr));
}





void up_layer(GtkButton *button,gpointer user_data){
    UserInterface *ui = user_data;
    GtkListBoxRow *curlbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button)))));
    GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(curlbr)));
    int pos =  gtk_list_box_row_get_index (curlbr);
  

    if (pos){
	//g_print("\n-------------pos : %i,nblayers = %i---------------\n",pos,ui->nblayers);
	
	swap_next_el_data(&Layers,pos-1);
	
	GtkListBoxRow * lastlbr =  GTK_LIST_BOX_ROW(gtk_list_box_get_row_at_index(lb,pos-1));
	
	GtkLabel *lastlabel = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(lastlbr))),1,0));
	const gchar* tmp = gtk_label_get_text(lastlabel);
	char *my_string;
    	int val = asprintf(&my_string,"%s",tmp);
    	if(val <0)
		errx(1,"cannot create the query");
	GtkLabel * currentlabel = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(curlbr))),1,0));
	gtk_label_set_text(lastlabel,gtk_label_get_text(currentlabel));
	gtk_label_set_text(currentlabel,my_string);
    }

}

void down_layer(GtkButton *button,gpointer user_data){
    UserInterface *ui = user_data;

    GtkListBoxRow *curlbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button)))));
    GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(curlbr)));
    int pos =  gtk_list_box_row_get_index (curlbr);

   
    if (pos<ui->nblayers-1){
       	//g_print("\n-------------pos : %i,nblayers = %i---------------\n",pos,ui->nblayers);
	swap_next_el_data(&Layers,pos);
	
	GtkListBoxRow * nextlbr =  GTK_LIST_BOX_ROW(gtk_list_box_get_row_at_index(lb,pos+1));
	GtkLabel *nextlabel = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(nextlbr))),1,0));
	const gchar* tmp = gtk_label_get_text(nextlabel);
	char *my_string;
    	int val = asprintf(&my_string,"%s",tmp);
    	if(val <0)
		errx(1,"cannot create the query");
	GtkLabel * currentlabel = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(curlbr))),1,0));
	gtk_label_set_text(nextlabel,gtk_label_get_text(currentlabel));
	gtk_label_set_text(currentlabel,my_string);

    }
}

void set_current_layer(GtkListBox *box ,GtkListBoxRow *listboxrow,gpointer user_data){
    
    UserInterface *ui = user_data;
    //int pos =  gtk_list_box_row_get_index (listboxrow);
 
    Layer * current_layer = elm_at_pos(&Layers,gtk_list_box_row_get_index (listboxrow));
    im = current_layer->im;
    actualise_image(im,0,0,im->width,im->height);
    gtk_image_set_from_pixbuf(ui->area,im->pb);
    prepare_drawarea(user_data);
    ui->currentLayer = current_layer;
    gtk_list_box_select_row (box,listboxrow);
    g_print("clicked on the %i element\n",get_index_layer(Layers,listboxrow));

    if (!sauv_im1){
	g_print("new image\n");
	sauv_im1 = new_image(im->width,im->height);
	sauv_im1 = copy_image(im,sauv_im1);

    }
    else{
    	sauv_im1 = copy_image(im,sauv_im1);
    }

}

void set_current_layer(GtkListBox *box ,GtkListBoxRow *listboxrow,gpointer user_data){
    
    UserInterface *ui = user_data;
    Layer * current_layer = elm_at_pos(&Layers,gtk_list_box_row_get_index (listboxrow));
    im = current_layer->im;
    im2 = current_layer->im;
    actualise_image(im,0,0,im->width,im->height);
    gtk_image_set_from_pixbuf(ui->area,im->pb);


}

void destroy_layer(GtkButton *button,gpointer user_data){
    UserInterface *ui = user_data;
    GtkListBoxRow *curlbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button))));
    int pos =  gtk_list_box_row_get_index (curlbr);

    gtk_widget_destroy(GTK_WIDGET(curlbr));

    Layer * dead = pop_from_stack_at_pos(&Layers,pos);
    free_image(dead->im);
    free(dead);
    ui->nblayers -=1;
    

}
//Impossible à faire marcher
gint trifunc(GtkListBoxRow *row1,GtkListBoxRow *row2,gpointer user_data){
	UserInterface *ui = user_data;
	int pos1 =  gtk_list_box_row_get_index (row1);
	int pos2 =  gtk_list_box_row_get_index (row2);
	g_print("init pos1 : %i , pos2 : %i \n",pos1,pos2);

	pos1 = get_index_layer(Layers, row1);
	pos2 = get_index_layer(Layers, row2);
	g_print("init pos1 : %i , pos2 : %i \n",pos1,pos2);
	
	return pos1>pos2 ? 1 : -1;

}


void add_layer(GtkButton *useless,gpointer user_data){
    UserInterface *ui = user_data;   

    // création de la box contenant les infos du layer
    GtkListBoxRow * nbr = GTK_LIST_BOX_ROW(gtk_list_box_row_new ());
   
    GtkGrid *box = GTK_GRID(gtk_grid_new ());
    gtk_grid_insert_column(box,0);
    gtk_grid_insert_column(box,0);
    gtk_grid_insert_column(box,0);
    gtk_grid_insert_column(box,0);


    //bouton hide/show
    GtkWidget *button = gtk_button_new_with_label ("show?");
    g_signal_connect(button, "clicked", G_CALLBACK(show_hide_layer), user_data);
    gtk_grid_attach (box,button,0,0,1,1);
    
    //nom du layer
    char *my_string;
    int val = asprintf(&my_string,"Layer %i",ui->nblayers);
    if(val <0)
		errx(1,"cannot create the query");
    GtkWidget *name = gtk_label_new(my_string);	
    gtk_widget_set_name(name, "label");
    gtk_grid_attach (box,name,1,0,1,1);

    //bouton up/down
    GtkWidget *bud = gtk_box_new (GTK_ORIENTATION_VERTICAL,1);
    GtkWidget *upbutton = gtk_button_new_with_label ("up");
    GtkWidget *downbutton = gtk_button_new_with_label ("down");
    gtk_button_set_relief(GTK_BUTTON(upbutton),GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(downbutton),GTK_RELIEF_NONE);
    g_signal_connect(upbutton, "clicked", G_CALLBACK(up_layer), user_data);
    g_signal_connect(downbutton, "clicked", G_CALLBACK(down_layer), user_data);
    gtk_container_add (GTK_CONTAINER(bud),upbutton);
    gtk_container_add (GTK_CONTAINER(bud),downbutton);
    gtk_grid_attach (box,bud,2,0,1,1);


    //bouton kill layer
    GtkWidget *killbutton = gtk_button_new_with_label ("kill");
    g_signal_connect(killbutton, "clicked", G_CALLBACK(destroy_layer), user_data);
    gtk_grid_attach (box,killbutton,3,0,1,1);


    gtk_container_add (GTK_CONTAINER(nbr),GTK_WIDGET(box));


    gtk_list_box_insert (ui->layers,GTK_WIDGET(nbr),0);
    //gtk_widget_hide (GTK_WIDGET(ui->layers));
    gtk_widget_show_all(GTK_WIDGET(ui->layers));
    
    struct Layer *newLayer = malloc(sizeof(struct Layer));
    if(!im)
	 newLayer->im = new_image(500,500);
    else
    	newLayer->im = new_image(im->width,im->height);
    newLayer->show = 0;
    newLayer-> lbr = nbr;
    ui->nblayers +=1;
    Layers = push_to_stack(Layers,newLayer);
    set_current_layer(ui->layers,nbr,user_data);
    

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

    GtkStack* stack_used = GTK_STACK(gtk_builder_get_object(builder,"stack1"));
      
    GtkTextBuffer* curser_position = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cursor_pos"));
    GtkAdjustment* print_width_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "width_value"));  
    GtkAdjustment* print_height_value =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "height_value"));  
    
    GtkAdjustment* draw_size =  GTK_ADJUSTMENT(gtk_builder_get_object(builder, "draw_size"));  
    GtkListBox * layers = GTK_LIST_BOX(gtk_builder_get_object(builder,"Layers"));
    GtkButton* UNDO_button = GTK_BUTTON(gtk_builder_get_object(builder, "Undo"));
    GtkButton* add_layer_button = GTK_BUTTON(gtk_builder_get_object(builder, "add_layer"));



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

    GtkRadioButton* brush1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "brush1"));
    GtkRadioButton* brush2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "brush2"));
    GtkRadioButton* brush3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "brush3"));


    struct _GdkRGBA *col = malloc(sizeof(struct _GdkRGBA));
    col->red = 0;
    col->blue = 0;
    col->green = 0;
    col->alpha = 255;
    gtk_color_chooser_set_rgba(draw_color,col);
    struct Pixel pixel = {0,0,0,255};
    //im = new_image(1650,900);
    //copy_image(im2,im);

   
    Layers = create_stack();

    

    UserInterface ui ={
                .window = window,
                .drawarea = drawarea,
                .stack_used = stack_used,
		.layers = layers,
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

		.brush1 = brush1,
		.brush2 = brush2,
		.brush3 = brush3,

		.draw_size = draw_size,
		.draw_value  = 1,
		.tolerance = 1,
		.nblayers = 0,
		.currentLayer = NULL,
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

    g_signal_connect(flood_fill,"clicked", G_CALLBACK(apply_swap_fill_mode), &ui);


    g_signal_connect(UNDO_button,"clicked", G_CALLBACK(apply_undo), &ui);


    g_signal_connect(CB_button, "clicked", G_CALLBACK(apply_color_balance), &ui);
    g_signal_connect(ROT_button, "clicked", G_CALLBACK(apply_rotation), &ui);   

    g_signal_connect(add_layer_button, "clicked", G_CALLBACK(add_layer), &ui);


    g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);
    g_signal_connect(saver, "clicked", G_CALLBACK(on_save), &ui);

    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);
    //g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_press), &ui);

    g_signal_connect(eb_draw, "motion-notify-event",G_CALLBACK (mouse_moved), &ui);
    g_signal_connect(eb_draw, "button_press_event",G_CALLBACK (mouse_clicked), &ui);
   
    //g_signal_connect(eb_draw, "scroll_event", G_CALLBACK( scroll_callback ), &ui);
    //
    g_signal_connect(draw_color,"color-set",G_CALLBACK(color_updated),&ui);

    g_signal_connect(layers, "row-activated", G_CALLBACK(set_current_layer), &ui);


    gtk_main();
    g_object_unref(builder);

    if (im){

    	free_image(im);
    	free(im);
    }
    /*
    if (sauv_im1){
    	free_image(sauv_im1);
    	free(sauv_im1);
    }*/
/*
    

    free_image(im2);
    free(im2);*/
    free_stack(Layers);
    // Exits.

    return 0;
    }
