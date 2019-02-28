#include <gtk/gtk.h>

#define WIDTH 50
#define HEIGHT 100



static void selecter(GtkWidget *widget, gpointer data){
	g_print ("Selection tool\n");
}

static void move(GtkWidget *widget, gpointer data){
	g_print ("Move tool\n");
}

static void pencil(GtkWidget *widget, gpointer data){
	g_print ("Pencil tool\n");
}

static void eraser(GtkWidget *widget, gpointer data){
	g_print ("Eraser tool\n");
}

static void text(GtkWidget *widget, gpointer data){
	g_print ("Text tool\n");
}

static void bucket(GtkWidget *widget, gpointer data){
	g_print ("Paint bucket tool\n");
}

static void picker(GtkWidget *widget, gpointer data){
	GtkWidget *colorWheel = gtk_color_chooser_dialog_new ("Colors", NULL);
	gtk_dialog_run(colorWheel);
	gtk_widget_destroy(colorWheel);
}


static void activate(GtkApplication *app, gpointer user_data){
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *button;
	GtkWidget *image;
	GdkPixbuf *pixbuf;

	/* create a new window, and set its title */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);

	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	
	gtk_container_set_border_width(GTK_CONTAINER (window), 10);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous(grid, 1);
	gtk_grid_set_column_homogeneous(grid, 1);

	/* Pack the container in the window */
	gtk_container_add (GTK_CONTAINER (window), grid);






	//TOOLS
	//Col, row
	//Selection Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(selecter), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/point.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Move Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(move), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/point.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Pencil Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(pencil), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/pencil.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Eraser Picker Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(eraser), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 1, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/eraser.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Text tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(text), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/text.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Paint Bucket Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(bucket), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 2, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/bucket.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Color Picker Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(picker), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/drop.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);


	/* Now that we are done packing our widgets, we show them all
	* in one go, by calling gtk_widget_show_all() on the window.
	* This call recursively calls gtk_widget_show() on all widgets
	* that are contained in the window, directly or indirectly.
	*/
	gtk_widget_show_all(window);
}

int main(int argc, char **argv){
	GtkApplication *app;
	gtk_init (&argc, &argv);
	int status;
	app = gtk_application_new("paint.c", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);

	return status;
}
