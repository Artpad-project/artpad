/*!
 *  File created on 9/4/2021 by tom.aubert
 *  Contributors : tom aubert
 *
 *  File containing all the lauching function for the image treatment in the UI.
 */

#include "basics_gui.h"
#include "image_tools.h"

/* Put the color balance*/
void apply_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));

    if (im){
        copy_image(im2,im);
        g_print("before balanceabsolue\n");
	BalanceAbsolue(im,gtk_adjustment_get_value(ui->CB_value));
        sleep(1);
        int draw_width= gtk_widget_get_allocated_width(GTK_WIDGET(ui->area));
        int draw_height = gtk_widget_get_allocated_height(GTK_WIDGET(ui->area));
        im_toprint = im;
	gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,draw_width,draw_height);

        
    }
}
/* Put the saturation*/
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
        im_toprint = im;
	gtk_widget_queue_draw_area(GTK_WIDGET(ui->area),0,0,draw_width,draw_height);
    }
}
/* Put the rotation*/
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
