#include "../../include/gui.h"

/* Automatically put the color balance*/
void apply_auto_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (ui->im){
        
        temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
	merge_to_death(ui->Layers,user_data);
        BalanceAuto(ui->currentLayer->im);
	merge_from_layers(ui->Layers, ui->im);

        draw_total_image(user_data);
    }
}




/* Put the color balance*/
void apply_color_balance(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);

    //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->CB_value)));
   
    if (ui->im){
        temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
	merge_to_death(ui->Layers,user_data);
        BalanceAbsolue(ui->currentLayer->im,gtk_adjustment_get_value(ui->CB_value));

        draw_total_image(user_data);
    }
}

/* Put saturation*/
void apply_saturation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
	merge_to_death(ui->Layers,user_data);
        // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        SaturationAbsolue(ui->currentLayer->im,gtk_adjustment_get_value(ui->SAT_value));

        draw_total_image(user_data);
    }
}

//brightness function
void apply_brightness(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
	merge_to_death(ui->Layers,user_data);
        // g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        Contrast(ui->currentLayer->im,gtk_adjustment_get_value(ui->CON_value),gtk_adjustment_get_value(ui->BRI_value));

	draw_total_image(user_data);
    }
}


//rotation function
void apply_rotation(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){

        
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
	Stack *tmp = ui->Layers;
	while (!is_stack_empty(tmp)){

		Layer * cur_layer = pop_from_stack(&tmp);
		if (cur_layer){
			temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
			Rotate(cur_layer->im,(float)gtk_adjustment_get_value(ui->ROT_value));
	
		}
	}
        merge_from_layers(ui->Layers, ui->im);
        draw_total_image(user_data);
    }
}

//right rotation
void apply_rot_right(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
        //free_image(im);
    if (ui->im){
	g_print("nblayers = %i\n",ui->nblayers);
       
	Stack *tmp = ui->Layers;
	while (!is_stack_empty(tmp)){
		Layer * cur_layer = pop_from_stack(&tmp);
		if (cur_layer){
			 g_print("un test\n");
			 temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
			 Rotate(cur_layer->im,(float)90);
	
		}
	}

        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
        merge_from_layers(ui->Layers, ui->im);
        draw_total_image(user_data);
    }
}

//left rotation
void apply_rot_left(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
       
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
	Stack *tmp = ui->Layers;
	while (!is_stack_empty(tmp)){
		Layer * cur_layer = pop_from_stack(&tmp);
		if (cur_layer){
			temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
			Rotate(cur_layer->im,(float)90);
		}
	}
	merge_from_layers(ui->Layers, ui->im);
	draw_total_image(user_data);
    }
}

//horizontal flip
void apply_flip_hori(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
	
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
	Stack *tmp = ui->Layers;

	while (!is_stack_empty(tmp)){
		Layer * cur_layer = pop_from_stack(&tmp);
		if (cur_layer){
			temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
		        FlipHorizon(cur_layer->im);
		}
	}

        draw_total_image(user_data);
    }
}

//vertical flip
void apply_flip_vert(GtkButton *button,gpointer user_data){

    UserInterface* ui = user_data;
    //free_image(im);
    if (ui->im){
        temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
	
        //g_print("%f\n",gtk_adjustment_get_value (GTK_ADJUSTMENT(ui->SAT_value)));
	Stack *tmp = ui->Layers;
	while (!is_stack_empty(tmp)){
	Layer * cur_layer = pop_from_stack(&tmp);
		if (cur_layer){
			temp_layer_push(ui->currentLayer->tp, ui->maxLayers, ui->currentLayer->im);
			FlipVertical(cur_layer->im);

		}
        draw_total_image(user_data);
    	}
    }
}



