#include "../../include/gui.h"

/* Automatically put the color balance*/
void apply_auto_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (ui->im){
        BalanceAuto(ui->im);

        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}




/* Put the color balance*/
void apply_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (ui->im){
        BalanceAbsolue(ui->im,gtk_adjustment_get_value(ui->CB_value));

        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}

/* Put saturation*/
void apply_saturation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        SaturationAbsolue(ui->im,gtk_adjustment_get_value(ui->SAT_value));

        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}

//brightness function
void apply_brightness(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Contrast(ui->im,gtk_adjustment_get_value(ui->CON_value),gtk_adjustment_get_value(ui->BRI_value));

        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}


//rotation function
void apply_rotation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
         Rotate(ui->im,(float)gtk_adjustment_get_value(ui->ROT_value));

        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}

//right rotation
void apply_rot_right(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    g_print("nblayers = %i\n",ui->nblayers);

    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(ui->im,(float)90);

        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}

//left rotation
void apply_rot_left(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
         //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(ui->im,(float)-90);
        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}

//horizontal flip
void apply_flip_hori(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        FlipHorizon(ui->im);
        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}

//vertical flip
void apply_flip_vert(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        FlipVertical(ui->im);
        actualise_image(ui->im,0,0,ui->im->width,ui->im->height);
        gtk_image_set_from_pixbuf(ui->area,ui->im->pb);
    }
}



