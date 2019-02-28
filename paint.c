#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500
#define COLOR_CHANNELS 4
#define CHAR_DEPTH 2

struct Image {
	int width, height;
	char ****data;
};

static void save_pdc(struct Image image)
{
	int i, j, k, l;
	FILE *output_file;

	output_file = fopen("wow.pdc", "w");

	fprintf(output_file, "%d", image.width);
	fputs("\n", output_file);
	fprintf(output_file, "%d", image.height);
	fputs("\n", output_file);

	for (i=0; i<image.width; i++) {
		for (j=0; j<image.height; j++) {
			for (k=0; k<COLOR_CHANNELS; k++) {
				for (l=0; l<CHAR_DEPTH; l++) {
					fputs(&image.data[i][j][k][l], output_file);
				}
			}
		}
	}

	fclose(output_file);
}

static void print_hello(GtkWidget *widget, gpointer data)
{
	g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *button;

	/* create a new window, and set its title */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();

	/* Pack the container in the window */
	gtk_container_add(GTK_CONTAINER(window), grid);

	button = gtk_button_new_with_label("Button 1");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

	/* Place the first button in the grid cell (0, 0), and make it fill
	* just 1 cell horizontally and vertically (ie no spanning)
	*/
	gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

	button = gtk_button_new_with_label("Button 2");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

	/* Place the second button in the grid cell (1, 0), and make it fill
	* just 1 cell horizontally and vertically (ie no spanning)
	*/
	gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

	button = gtk_button_new_with_label("Quit");
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

	/* Place the Quit button in the grid cell (0, 1), and make it
	* span 2 columns.
	*/
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

	/* Now that we are done packing our widgets, we show them all
	* in one go, by calling gtk_widget_show_all() on the window.
	* This call recursively calls gtk_widget_show() on all widgets
	* that are contained in the window, directly or indirectly.
	*/
	gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;
	struct Image image;
	int i, j, k, l;

	image.width = DEFAULT_WIDTH;
	image.height = DEFAULT_HEIGHT;
	image.data = (char ****) malloc(image.width * sizeof(char ***));

	for (i=0; i<image.width; i++) {
		image.data[i] = (char ***) malloc(image.height * sizeof(char **));

		for (j=0; j<image.width; j++) {
			image.data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));

			for (k=0; k<COLOR_CHANNELS; k++) {
				image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));

				for (l=0; l<CHAR_DEPTH; l++) {
					image.data[i][j][k][l] = 'f';
				}
			}
		}
	}

	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);

	save_pdc(image);

	return status;
}