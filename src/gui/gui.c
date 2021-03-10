#include <gtk/gtk.h>
#include "../image/image.c"

// Structure of the graphical user interface.

Image* im ;

typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkDrawingArea* area;           // Drawing area
    GtkButton* start_button;        // Start button
    GdkRectangle selectzone;
} UserInterface;

// Event handler for the "draw" signal of the drawing area.

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    // Gets the 'Game' structure.

    //UserInterface *ui = user_data;

    // Sets the background to white.

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    if (im){
        Pixel pixel;
        int draw_width=1000;
        int draw_height = 600;
        float r, g, b;

        for (int x = 0; x < im->width; x++) {
            for (int y = 0; y < im->height; y++) {
                pixel = im->pixels[x][y];
                r = (float) pixel.red / 255;
                g = (float) pixel.green / 255;
                b = (float) pixel.blue / 255;
                cairo_set_source_rgb(cr, r, g, b);
                cairo_rectangle(cr, x+(draw_width/2 - im->width/2), (draw_height/2 - im->height/2) + y,1,1);
        	    cairo_fill(cr);
            }
        }

    }
   
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

void on_load(GtkFileChooser *fc,gpointer user_data){
    //g_print("load\n");
    UserInterface* ui = user_data;
    im = load_image((char *)gtk_file_chooser_get_filename (fc));
    
}


// Event handler for the "clicked" signal of the start button.

void on_start(GtkButton *button, gpointer user_data)
{
    UserInterface *ui = user_data;
    ui->selectzone.x += 100;
    g_print("%i\n",ui->selectzone.x);

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
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "copy"));
    GtkFileChooser* loader =  GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "loader"));  
    // Creates the "Game" structure.

    UserInterface ui ={
                .window = window,
                .area = area,
                .start_button = start_button,
                .selectzone = {0,0,0,0},
    };
    // Connects event handlers.
    // Runs the main loop.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(area, "draw", G_CALLBACK(on_draw), &ui);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &ui);
    if (loader != NULL){ 
        g_print("yesay");
        g_signal_connect(loader, "file_set", G_CALLBACK(on_load), &ui);
    }
    
    gtk_main();

    // Exits.

    return 0;
}
