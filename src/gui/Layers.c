/*!
 *  File created on 13/5/2021 by tom aubert
 *  Contributors : tom aubert
 *
 *  File containing all the functions necessary to interact with Layers.
 * 
 */

#include "../../include/Layers.h"
#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <string.h>
#include "../../include/gui.h"

//length of "_layers00\0"
#define APPEND_LENGTH 10
#define _GNU_SOURCE

/// FONCTIONS USED FOR GUI


void show_hide_layer(GtkButton *button,gpointer user_data)
{
    UserInterface* ui = user_data;
    GtkListBoxRow *lbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button))));
    //GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(lbr)));

    int pos =  gtk_list_box_row_get_index (lbr);
    Layer* current_layer = elm_at_pos(&ui->Layers,pos);
    if (current_layer->show){

	GError* err = NULL;
    	GdkPixbuf * buf = gdk_pixbuf_new_from_file("src/gui/icons/pasvue.png",&err);
    	if (buf == NULL)
		errx(err->code,"ERROR: image.c: couldn't load pixbuf (%s)",err->message);
	GtkWidget *vue = gtk_image_new_from_pixbuf(buf);
    	gtk_button_set_image(GTK_BUTTON(button),vue);

    	current_layer->show = 0;
    }
    else if (!current_layer->show){
	GError* err = NULL;
    	GdkPixbuf * buf = gdk_pixbuf_new_from_file("src/gui/icons/vue.png",&err);
    	if (buf == NULL)
		errx(err->code,"ERROR: image.c: couldn't load pixbuf (%s)",err->message);
    	GtkWidget *vue = gtk_image_new_from_pixbuf(buf);
    	gtk_button_set_image(GTK_BUTTON(button),vue);

   	current_layer->show = 1;
    }
    merge_from_layers(ui->Layers,ui->im);
    draw_total_image(user_data);
}


void hide_all_layers(GtkButton* buttonuseless, gpointer user_data){
	UserInterface* ui = user_data;
	Stack * curlayer = ui->Layers;
	while (!is_stack_empty(curlayer)){
		Layer * current_layer = (Layer *)curlayer->data;
		if (current_layer->show){
		    
		    GError* err = NULL;
		    GdkPixbuf * buf = gdk_pixbuf_new_from_file("src/gui/icons/pasvue.png",&err);
		    if (buf == NULL)
			errx(err->code,"ERROR: image.c: couldn't load pixbuf (%s)",err->message);
		    GtkWidget *vue = gtk_image_new_from_pixbuf(buf);
		    gtk_button_set_image(GTK_BUTTON(current_layer->button),vue);
		    current_layer->show = 0;
	        }
		curlayer = curlayer->next;

	}
 	merge_from_layers(ui->Layers,ui->im);
    	draw_total_image(user_data);
}

void show_all_layers(GtkButton* buttonuseless, gpointer user_data){
	UserInterface* ui = user_data;
	Stack * curlayer = ui->Layers;
	while (!is_stack_empty(curlayer)){
		Layer * current_layer = (Layer *)curlayer->data;

		if (!current_layer->show){
			GError* err = NULL;
			GdkPixbuf * buf = gdk_pixbuf_new_from_file("src/gui/icons/vue.png",&err);
			if (buf == NULL)
				errx(err->code,"ERROR: image.c: couldn't load pixbuf (%s)",err->message);
			GtkWidget *vue = gtk_image_new_from_pixbuf(buf);
			gtk_button_set_image(GTK_BUTTON(current_layer->button),vue);
			current_layer->show = 1;
		}
		curlayer = curlayer->next;

	}
        merge_from_layers(ui->Layers,ui->im);
        draw_total_image(user_data);
}


void up_layer(GtkButton *button,gpointer user_data){
    UserInterface *ui = user_data;
    GtkListBoxRow *curlbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button)))));
    GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(curlbr)));
    int pos =  gtk_list_box_row_get_index (curlbr);
  

    if (pos)
    {
        //g_print("\n-------------pos : %i,nblayers = %i---------------\n",pos,ui->nblayers);
        
        swap_next_el_data(&ui->Layers,pos-1);
        
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
 	merge_from_layers(ui->Layers,ui->im);
    	actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
    	gtk_image_set_from_pixbuf(ui->area,ui->im->pb);

    }
}

void down_layer(GtkButton *button,gpointer user_data){
    UserInterface *ui = user_data;

    GtkListBoxRow *curlbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button)))));
    GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(curlbr)));
    int pos =  gtk_list_box_row_get_index (curlbr);

   
    if (pos<ui->nblayers-1)
    {
      //g_print("\n-------------pos : %i,nblayers = %i---------------\n",pos,ui->nblayers);
      swap_next_el_data(&ui->Layers,pos);
	
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
      merge_from_layers(ui->Layers,ui->im);
      actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
      gtk_image_set_from_pixbuf(ui->area,ui->im->pb);

    }
}

void set_current_layer(GtkListBox *box ,GtkListBoxRow *listboxrow,gpointer user_data){
    
    UserInterface *ui = user_data;
    //int pos =  gtk_list_box_row_get_index (listboxrow);

    //g_print("%i\n",gtk_list_box_row_get_index (listboxrow));
    Layer * current_layer = elm_at_pos(&ui->Layers,gtk_list_box_row_get_index (listboxrow));
    
    merge_from_layers(ui->Layers,ui->im);

    draw_total_image(user_data);

    prepare_drawarea(user_data);

    ui->currentLayer = current_layer;
    gtk_list_box_select_row (box,listboxrow);

    //g_print("clicked on the %i element\n",get_index_layer(Layers,listboxrow));
}

void destroy_layer(GtkButton *button,gpointer user_data){
    UserInterface *ui = user_data;
    GtkListBoxRow *curlbr = GTK_LIST_BOX_ROW(gtk_widget_get_parent (gtk_widget_get_parent (GTK_WIDGET(button))));
    //GtkListBox * lb = GTK_LIST_BOX(gtk_widget_get_parent (GTK_WIDGET(curlbr)));

    int pos =  gtk_list_box_row_get_index (curlbr);


    gtk_widget_destroy(GTK_WIDGET(curlbr));
   
    Layer * dead = pop_from_stack_at_pos(&ui->Layers,pos);
    dead->lbr = 0;
    free_layer(dead);

//todo : need to set the new image
    Layer * new = elm_at_pos(&ui->Layers,0);
    if (new != NULL)
    {
	    set_current_layer(ui->layers,GTK_LIST_BOX_ROW(gtk_list_box_get_row_at_index (ui->layers,0)),user_data);
    	    merge_from_layers(ui->Layers,ui->im);
	    draw_total_image(user_data);    	    
    }

    else {
	    merge_from_layers(ui->Layers,ui->im);
    	    actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
            gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
	    free_image(ui->im);
	    ui->im = NULL;
	    ui->currentLayer = NULL;
    }

    ui->nblayers -=1;

}

//Impossible à faire marcher
gint trifunc(GtkListBoxRow *row1,GtkListBoxRow *row2,gpointer user_data){
	UserInterface *ui = user_data;
	int pos1 =  gtk_list_box_row_get_index (row1);
	int pos2 =  gtk_list_box_row_get_index (row2);
	g_print("init pos1 : %i , pos2 : %i \n",pos1,pos2);

	pos1 = get_index_layer(ui->Layers, row1);
	pos2 = get_index_layer(ui->Layers, row2);
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

    GtkWidget *button = gtk_button_new();

    GError* err = NULL;
    GdkPixbuf * buf = gdk_pixbuf_new_from_file("src/gui/icons/vue.png",&err);
    if (buf == NULL)
	    g_print("deuuuuuh\n");
    GtkWidget *vue = gtk_image_new_from_pixbuf(buf);
    gtk_button_set_image(GTK_BUTTON(button),vue);
    g_signal_connect(button, "clicked", G_CALLBACK(show_hide_layer), user_data);
    g_object_unref(buf);

    gtk_grid_attach (box,button,0,0,1,1);
    
    //nom du layer
    char *my_string;
    int val = asprintf(&my_string,"Layer %i",ui->nblayers);
    if(val <0)
		  errx(1,"cannot create the query");
    //GtkEntryBuffer * buf= gtk_entry_buffer_new (my_string,val);
   
    /* GtkWidget* entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 64);
    gtk_entry_set_text(GTK_ENTRY(entry), "Enter a name");
    gtk_entry_set_visibility(GTK_ENTRY(entry), TRUE);
    gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);*/


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

    //todo : optmise this sheat
	

    if(!ui->im){
	    newLayer->im = new_image(gtk_adjustment_get_value(ui->width_print),gtk_adjustment_get_value(ui->height_print));
	    ui->im = create_copy_image(newLayer->im);
    }
    else
      newLayer->im = new_image(ui->im->width,ui->im->height);

    newLayer->tp = malloc(sizeof(struct temp_layer));
    newLayer->tp->layers_y = NULL;
    newLayer->tp->layers_z = NULL;
    newLayer->tp->n = 0;


    newLayer->show = 1;
    newLayer-> lbr = nbr;
    newLayer->button = GTK_BUTTON(button);
    ui->nblayers +=1;
    ui->Layers = push_to_stack(ui->Layers,newLayer);
    set_current_layer(ui->layers,nbr,user_data);
}


/// OTHER FONCTIONS 
/* get the index of a layer in the stack from it's gtklistboxrow
 * 
 * @param 
 * Layers: the stack
 * lbr : the gtklistboxrow
 */


int get_index_layer(Stack* Layers,GtkListBoxRow * lbr)
{
	Stack * tmp = Layers;
	int pos = 0;

	if (!is_stack_empty(tmp)){
		//_print("whaouh\n");

		while (tmp->next != NULL )
    		{
			Layer * act = tmp->data;
			if (act->lbr == lbr)			
				break;
			pos ++;
			tmp = tmp->next;
		}
	}
	return pos;
}

/*!
 *   give the render of all the selected layers
 */

void merge_from_layers(Stack* Layers,struct Image* im){


    /*int** matrix = malloc(im->width * sizeof(int*));
    for (int i = 0; i < im->width; i++) {
	 matrix[i] = malloc(im->height * sizeof(int)); 
    	 memset(im->pixels[i], 255, im->height*sizeof(struct Pixel));
    }*/

    
    for(int i = 0;i<im->width;i++)
        for(int j = 0;j<im->height;j++){
            im->pixels[i][j].alpha = 0;
            im->pixels[i][j].blue = 255;
            im->pixels[i][j].green = 255;
            im->pixels[i][j].red = 255;
        }
           

    Stack* layers = Layers;
    while (!is_stack_empty(layers)){
	    Layer* cur_layer = layers->data;
	    if (cur_layer->show)
        {    
		    for(int i = 0;i<im->width;i++){
			    for(int j = 0;j<im->height;j++){
			      if (im->pixels[i][j].alpha<255){
				      int alpha = cur_layer->im->pixels[i][j].alpha;
				      if (im->pixels[i][j].alpha + alpha > 255)
					      alpha = 255 - im->pixels[i][j].alpha;

              im->pixels[i][j].red -= (255-cur_layer->im->pixels[i][j].red) * alpha/255;
              im->pixels[i][j].blue -= (255-cur_layer->im->pixels[i][j].blue) * alpha/255;
              im->pixels[i][j].green -= (255-cur_layer->im->pixels[i][j].green) * alpha/255;
              im->pixels[i][j].alpha += alpha;
			      }
          }
		    }
	    }
	    layers = layers->next;
    }
}

void apply_to_all_layers(void (*function) (void* ,void*),void* arg1,void* arg2, Stack* Layers){
	Stack * curlayer = Layers;
	while (!is_stack_empty(curlayer)){
		function(arg1,arg2);
	}
}


void free_layer(Layer* dead){
    if (dead->lbr)
	gtk_widget_destroy(GTK_WIDGET(dead->lbr));
    if (dead->im){
    	g_print("je suis la sécurité\n");
	free_image(dead->im);
	free(dead->im);
    }
    free(dead);

}



/* saves all layers in a subdirectory layers
 * the nameplate is "{dir_name}/{img original name}_layers00.{filetype};"
 * @param 
 * img: the original image
 * layers: the stack of layers
 * n: the number of layers in the stack
 */


void export(Image *img, Stack *layers, int n)
{
  char *buffer;
  size_t dir_length = 0;

  //creates directory
  if (!is_stack_empty(layers))
  {
    asprintf(&buffer, "draft_%c", img->file);
    // write permission only
    if (mkdir(buffer, S_IWRITE) == -1) 
      errx(EXIT_FAILURE, "couldn't create a directory\n");

    dir_length = strlen(buffer);
  }

  //saves directory
  while(!is_stack_empty(layers))
  {
    // Attention, ici tu recois une struc Layers, pas une struct Image
    Image *draft_img = pop_from_stack(&layers);
    //struct Layer * cur_layer = pop_from_stack(&layers);
    //struct Image * draft_img = cur_layer->im;
    size_t length = strlen(draft_img->file);

    //+1 accounts for the "/" separation
    char draft_path[length + APPEND_LENGTH + dir_length + 1];
    strncat(draft_path, buffer, dir_length);
    strcat(draft_path, "/");
    strncat(draft_path, img->file, dir_length);

    asprintf(&buffer, "_layers%d", n);
    n--;

    strcat(draft_path, buffer);

    save_image(draft_img, draft_path, draft_img->file_type);
  }
}
