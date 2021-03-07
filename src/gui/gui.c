#include <gtk/gtk.h>



#define PADDLE_STEP 5               // Step of a paddle in pixels

#define PADDLE_PERIOD 5             // Period of a paddle in milliseconds

#define DISC_PERIOD 4               // Period of the disc in milliseconds

#define END_GAME_SCORE 5            // Maximum number of points for a player



// State of the game.



// Structure of the graphical user interface.

typedef struct UserInterface

{

    GtkWindow* window;              // Main window

    GtkDrawingArea* area;           // Drawing area

    GtkButton* start_button;        // Start button


} UserInterface;





// Event handler for the "draw" signal of the drawing area.

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)

{

    // Gets the 'Game' structure.

    //UserInterface *ui = user_data;



    // Sets the background to white.

    cairo_set_source_rgb(cr, 1, 1, 1);

    cairo_paint(cr);



    // Draws the disc in red.
/*
    cairo_set_source_rgb(cr, 1, 0, 0);

    cairo_rectangle(cr, game->disc.rect.x, game->disc.rect.y,

        game->disc.rect.width, game->disc.rect.height);

    cairo_fill(cr);
*/


    // Propagates the signal.

    return FALSE;

}



// Event handler for the "clicked" signal of the start button.

void on_start(GtkButton *button, gpointer user_data)

{

    g_print("on_start()\n");

}



int main (int argc, char *argv[])

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



    // Gets the widgets.

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main"));
    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "Draw"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "cut"));
   


    // Creates the "Game" structure.

  

    UserInterface ui ={

                .window = window,

                .area = area,

                .start_button = start_button,

    };


    // Connects event handlers.

    // Runs the main loop.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(area, "draw", G_CALLBACK(on_draw), &ui);

    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &ui);

    gtk_main();



    // Exits.

    return 0;
}
