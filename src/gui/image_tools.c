#include "../../include/gui.h"

/* Automatically put the color balance*/
void apply_auto_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (ui->im){
        BalanceAuto(ui->im);

        draw_total_image(user_data);
    }
}




/* Put the color balance*/
void apply_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (ui->im){
        BalanceAbsolue(ui->im,gtk_adjustment_get_value(ui->CB_value));

        draw_total_image(user_data);
    }
}

/* Put saturation*/
void apply_saturation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        SaturationAbsolue(ui->im,gtk_adjustment_get_value(ui->SAT_value));

        draw_total_image(user_data);
    }
}

//brightness function
void apply_brightness(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Contrast(ui->im,gtk_adjustment_get_value(ui->CON_value),gtk_adjustment_get_value(ui->BRI_value));
	draw_total_image(user_data);
    }
}


//rotation function
void apply_rotation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
         Rotate(ui->im,(float)gtk_adjustment_get_value(ui->ROT_value));

        draw_total_image(user_data);
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

        draw_total_image(user_data);

    }
}

//left rotation
void apply_rot_left(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
         //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Rotate(ui->im,(float)-90);
        draw_total_image(user_data);
    }
}

//horizontal flip
void apply_flip_hori(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        FlipHorizon(ui->im);
        draw_total_image(user_data);
    }
}

//vertical flip
void apply_flip_vert(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        FlipVertical(ui->im);
        draw_total_image(user_data);
    }
}



