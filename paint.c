#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include "paint.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define COLOR_CHANNELS 4
#define CHAR_DEPTH 2


struct Image* image_ptr;



/* Takes a char pointer to a 2-digit hexidecimal number and returns its integer value.
 *
 * Hex value must be exactly two characters, and only contain numbers 0-9
 * and lowercase letters a-f.
 */
int hex2d(char* hex) {

	int total = 0;
	for (int i = 0; i < 2; i++) {

		switch (hex[i]) {

			// Increment total based on new digit
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				total *= 16;
				total += hex[i] - '0';
				break;

			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
				total *= 16;
				total += hex[i] - 'a' + 10;
				break;

		}
	}
	return total;
}

/* Converts an integer value into its hexidecimal equivalent, and returns a
 * pointer to a memory-allocated char array. Must be freed by caller.
 */
char* decimal_to_hex(int n) {
	int i, digit;
	char *hex = (char*) malloc(sizeof(char) * (CHAR_DEPTH + 1));

	for (i=0; i<CHAR_DEPTH; i++) {
		digit = n % 16;

		if (digit < 10) {
			hex[CHAR_DEPTH - i - 1] = 48 + digit;
		} else {
			hex[CHAR_DEPTH - i - 1] = 87 + digit;
		}

		n /= 16;
	}

	return hex;
}

/* Updates one pixel in the image with the values passed */
// NOTE: Assumes 4 color channel values
void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a) {
	char *red = decimal_to_hex(r);
	char *green = decimal_to_hex(g);
	char *blue = decimal_to_hex(b);
	char *alpha = decimal_to_hex(a);

	image.data[x][y][0][0] = red[0];
	image.data[x][y][0][1] = red[1];
	image.data[x][y][1][0] = green[0];
	image.data[x][y][1][1] = green[1];
	image.data[x][y][2][0] = blue[0];
	image.data[x][y][2][1] = blue[1];
	image.data[x][y][3][0] = alpha[0];
	image.data[x][y][3][1] = alpha[1];

	free(red);
	free(green);
	free(blue);
	free(alpha);
}

static void draw_line(int x1, int y1, int x2, int y2) {
	int i;

	for(i=0; i<abs(x1-x2); i++) {
		//update_pixel(*image_ptr, int(x1 - float(i)/float(x1-x2)));
	}
}

void brush_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
	printf("%ld\n", time(0));

	if (event->state & GDK_BUTTON1_MASK) {
		update_pixel(*image_ptr, event->x, event->y, 0, 0, 0, 255);
		update_pixel(*image_ptr, event->x+1, event->y, 0, 0, 0, 255);
		update_pixel(*image_ptr, event->x, event->y+1, 0, 0, 0, 255);
		update_pixel(*image_ptr, event->x+1, event->y+1, 0, 0, 0, 255);
		//gtk_widget_queue_draw_area(widget, event->x, event->y, 1, 1);
	}
}

/* Load an image from a pdc file */
struct Image load_pdc(char *file_name) {
	int input_char;
	int width = 0, height = 0, i = 0, j = 0, k = 0, l = 0;
	struct Image image;
	FILE *input_file;

	input_file = fopen(file_name, "r");

	// Read width from PDC file header
	while((input_char = getc(input_file)) != '\n') {
		width = 10*width + input_char - 48;
	}

	// Read height from PDC file header
	while((input_char = getc(input_file)) != '\n') {
		height = 10*height + input_char - 48;
	}

	image.width = width;
	image.height = height;

	// Allocate initial character arrays
	image.data = (char ****) malloc(image.width * sizeof(char ***));
	image.data[0] = (char ***) malloc(image.height * sizeof(char **));
	image.data[0][0] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
	image.data[0][0][0] = (char *) malloc(CHAR_DEPTH * sizeof(char));

	// Populate entire image array by reading one character at a time from the pdc file
	while((input_char = getc(input_file)) != EOF) {
		image.data[i][j][k][l] = input_char;

		// If we reach the character depth limit, move on to next color channel
		if (l == CHAR_DEPTH - 1) {
			l = 0;
			k++;
			image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		} else {
			l++;
		}

		// If we reach the last color channel, move on to the next column
		if (k == COLOR_CHANNELS) {
			k = 0;
			j++;
			image.data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
			image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		}

		// If we reach the end of a column, move on to the next row
		if (j == image.height) {
			j = 0;
			i++;
			image.data[i] = (char ***) malloc(image.height * sizeof(char **));
			image.data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
			image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		}
	}

	fclose(input_file);

	return image;
}

/*
	Save the image argument as the file name in pdc format

	PDC format:
	WIDTH\n
	HEIGHT\n
	DATA

	NOTE: There are no seperations in the data of the image file, ie no tabs, spaces, or newlines
*/
void save_pdc(struct Image image, char *file_name) {
	int i, j, k, l;
	FILE *output_file;

	output_file = fopen(file_name, "w");

	fprintf(output_file, "%d", image.width);
	fputs("\n", output_file);
	fprintf(output_file, "%d", image.height);
	fputs("\n", output_file);

	for (i=0; i<image.width; i++) {

		for (j=0; j<image.height; j++) {

			for (k=0; k<COLOR_CHANNELS; k++) {

				for (l=0; l<CHAR_DEPTH; l++) {

					fprintf(output_file, "%c", image.data[i][j][k][l]);

				}
			}
		}
	}

	fclose(output_file);
}

/* Create and allocate memory for a filled image of the specified width and height */
struct Image initialize_image(int width, int height) {
	int i, j, k, l;
	struct Image image;

	image.width = width;
	image.height = height;
	image.data = (char ****) malloc(image.width * sizeof(char ***));

	for (i=0; i<image.width; i++) {
		image.data[i] = (char ***) malloc(image.height * sizeof(char **));

		for (j=0; j<image.width; j++) {
			image.data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));

			for (k=0; k<COLOR_CHANNELS; k++) {
				image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));

				for (l=0; l<CHAR_DEPTH; l++) {

					// Fill white
					image.data[i][j][k][l] = 'f';

				}
			}
		}
	}

	return image;
}

/* Draw a single RGBA pixel on the canvas at position x, y */
gboolean render_pixel(GtkWidget* canvas, cairo_t* cr, int x, int y, int r, int g, int b, int a) {

	GdkRGBA color;
	color.red = r/255.0;
	color.green = g/255.0;
	color.blue = b/255.0;
	color.alpha = a/255.0;
	gdk_cairo_set_source_rgba (cr, &color);
	cairo_rectangle(cr, x, y, 1, 1);
	cairo_fill(cr);

	return FALSE;

}

gboolean update_canvas(GtkWidget* canvas, cairo_t *cr, gpointer data) {

	// Initialize variables for height, width, and color
	guint width, height, img_width, img_height;
	GdkRGBA color;
	GtkStyleContext* context;

	// Read context, width, and height from canvas struct
	context = gtk_widget_get_style_context(canvas);
	width = gtk_widget_get_allocated_width(canvas);
	height = gtk_widget_get_allocated_height(canvas);

	// Draw the weird, grayish background
	gtk_render_background(context, cr, 0, 0, width, height);

	img_width = (*image_ptr).width;
	img_height = (*image_ptr).height;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int c[COLOR_CHANNELS];
			for (int k = 0; k < COLOR_CHANNELS; k++) {
				c[k] = hex2d(((*image_ptr).data)[i][j][k]);
			}
			render_pixel(canvas, cr, i, j, c[0], c[1], c[2], c[3]);
		}
	}

	return FALSE;

}

void activate(GtkApplication *app, gpointer user_data) {

	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *button;
	GtkWidget *canvas;
	GtkWidget *canvas_grid;

	/* create a new window, and set its title */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();

	/* Pack the container in the window */
	gtk_container_add(GTK_CONTAINER(window), grid);

	button = gtk_button_new_with_label("Quit");
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

	/* Place the Quit button in the grid cell (0, 1), and make it
	* span 2 columns.
	*/
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

	/* Create canvas as drawing_area object, and set size */
	canvas = gtk_drawing_area_new();
	gtk_widget_set_events(canvas, gtk_widget_get_events(canvas) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	gtk_widget_set_size_request(canvas, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
	gtk_grid_attach(GTK_GRID(grid), canvas, 2, 2, 1, 1);
	g_signal_connect(G_OBJECT(canvas), "draw",
									G_CALLBACK(update_canvas), NULL);

	g_signal_connect(canvas, "motion-notify-event", G_CALLBACK(brush_mouse_motion), NULL);

	/* Now that we are done packing our widgets, we show them all
	* in one go, by calling gtk_widget_show_all() on the window.
	* This call recursively calls gtk_widget_show() on all widgets
	* that are contained in the window, directly or indirectly.
	*/
	gtk_widget_show_all(window);
}

int main(int argc, char **argv) {

	GtkApplication *app;

	int status;
	struct Image image;
	image_ptr = &image;

	image = initialize_image(500, 500);

	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);

	save_pdc(image, "wow.pdc");

	return status;
}
