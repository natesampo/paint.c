#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "paint.h"
#include "toolbar.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define COLOR_CHANNELS 4
#define CHAR_DEPTH 2


struct Image* image_ptr;
int tool = 0;
GdkRGBA color;
struct byteColor curr_color;
GtkWidget *canvas;

int lastX = -1;
int lastY = -1;
int brush_size = 6;

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((Y) < (X)) ? (X) : (Y))


/* Takes a char pointer to a 2-digit hexidecimal number and returns its integer value.
 *
 * Hex value must be exactly CHAR_DEPTH characters, and only contain numbers 0-9
 * and lowercase letters a-f.
 */
int hex_to_decimal(char* hex) {

	int total = 0;
	for (int i = 0; i < CHAR_DEPTH; i++) {

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

int check_bounds(struct Image image, int x, int y) {
	if ((x >= 0 && x < image.width) && (y >= 0 && y < image.height)) {
		return 1;
	} else {
		return 0;
	}
}

/* Updates one pixel in the image with the values passed */
// NOTE: Assumes 4 color channel values
void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a) {
	if (check_bounds(image, x, y)) {
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

	if (x >= 0 && x < gtk_widget_get_allocated_width(canvas) &&
		y >= 0 && y < gtk_widget_get_allocated_height(canvas)) {

		gtk_widget_queue_draw_area(canvas, x, y, 1, 1);
	}
}

double get_distance(int x1, int y1, int x2, int y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

void draw_line(struct Image image, int x1, int y1, int x2, int y2) {
	double distance = get_distance(x1, y1, x2, y2);
	int i;
	int cx;
	int cy;

	for(i=0; i<distance; i++) {
		cx = x1 - (int) ((x1 - x2)*(i/distance));
		cy = y1 - (int) ((y1 - y2)*(i/distance));
		draw_circle(image, cx, cy, brush_size);
	}
}

void paint_bucket_recurse(struct byteColor *temp_color, int **visited, int x, int y, int origin_x, int origin_y) {

	if (check_bounds(*image_ptr, x, y) &&
		!visited[x][y] &&
		hex_to_decimal(image_ptr->data[x][y][0]) == temp_color->red &&
		hex_to_decimal(image_ptr->data[x][y][1]) == temp_color->green &&
		hex_to_decimal(image_ptr->data[x][y][2]) == temp_color->blue &&
		hex_to_decimal(image_ptr->data[x][y][3]) == temp_color->alpha) {

		visited[x][y] = 1;
		update_pixel(*image_ptr, x, y, curr_color.red, curr_color.green, curr_color.blue, curr_color.alpha);

		paint_bucket_recurse(temp_color, visited, x+1, y, origin_x, origin_y);
		paint_bucket_recurse(temp_color, visited, x, y+1, origin_x, origin_y);
		paint_bucket_recurse(temp_color, visited, x-1, y, origin_x, origin_y);
		paint_bucket_recurse(temp_color, visited, x, y-1, origin_x, origin_y);
	}
}

void paint_bucket_mouse_clicked(GtkWidget *widget, GdkEventMotion *event, gpointer data) {

	if (check_bounds(*image_ptr, (int) event->x, (int) event->y)) {

		struct byteColor temp_color;
		temp_color.red = hex_to_decimal(image_ptr->data[(int) event->x][(int) event->y][0]);
		temp_color.green = hex_to_decimal(image_ptr->data[(int) event->x][(int) event->y][1]);
		temp_color.blue = hex_to_decimal(image_ptr->data[(int) event->x][(int) event->y][2]);
		temp_color.alpha = hex_to_decimal(image_ptr->data[(int) event->x][(int) event->y][3]);

		int i;
		int **visited = (int **) malloc(image_ptr->width * sizeof(int *));
		for (i=0; i<image_ptr->width; i++) {
			visited[i] = (int *) calloc(image_ptr->height, sizeof(int));
		}

		paint_bucket_recurse(&temp_color, visited, (int) event->x, (int) event->y, (int) event->x, (int) event->y);

		for (i=0; i<image_ptr->width; i++) {
			free(visited[i]);
		}
		free(visited);
	}
}

/* Draws a circle to the image structure at position (x, y) of diameter d.
	 Uses the color defined by curr_color.
	 TODO add hardness and antialiasing
*/
void draw_circle(struct Image image, int x, int y, int d) {

	int min_x = x - d/2;
	int min_y = y - d/2;
	int max_x = x + d/2;
	int max_y = y + d/2;
	int dist;

	for (int this_x = min_x; this_x <= max_x; this_x++) {
		for (int this_y = min_y; this_y <= max_y; this_y++) {

			dist = pow(this_x - x, 2) + pow(this_y - y, 2);
			if (dist <= pow(d, 2)/4) {
				update_pixel(image, this_x, this_y,
					curr_color.red, curr_color.green, curr_color.blue, curr_color.alpha);
			}
		}
	}

}

void line_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data) {

	if (event->state & GDK_BUTTON1_MASK) {
		draw_line(*image_ptr, lastX, lastY, event->x, event->y);
	}

	lastX = event->x;
	lastY = event->y;
}

void brush_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data) {

	if (event->state & GDK_BUTTON1_MASK) {
		draw_line(*image_ptr, lastX, lastY, event->x, event->y);

		int low_x = MIN(event -> x, lastX);
		int low_y = MIN(event -> y, lastY);
		int high_x = MAX(event -> x, lastX);
		int high_y = MAX(event -> y, lastY);
		int width = MAX(high_x - low_x, 1);
		int height = MAX(high_y - low_y, 1);
		int b = brush_size/2;	//	Border around draw area for buffer.
		//gtk_widget_queue_draw_area(widget,
		//		MAX(low_x - b, 0),
		//		MAX(low_y - b, 0),
		//		MIN(width + 2*b, image_ptr -> width - 1),
		//		MIN(height + 2*b, image_ptr -> height - 1));

	}

	lastX = event->x;
	lastY = event->y;
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

	// Convert color values from [0, 255] to [0, 1]
	GdkRGBA color;
	color.red = r/255.0;
	color.green = g/255.0;
	color.blue = b/255.0;
	color.alpha = a/255.0;

	// Draw a one-pixel rectangle onto the screen
	gdk_cairo_set_source_rgba (cr, &color);
	cairo_rectangle(cr, x, y, 1, 1);
	cairo_fill(cr);
	return FALSE;


}

/* Draws the pixels of the image onto the window to display the image. */
gboolean update_canvas(GtkWidget* canvas, cairo_t *cr, gpointer data) {

	// Initialize variables for height, width, and color
	guint width, height;
	GtkStyleContext* context;

	// Read context, width, and height from canvas struct
	context = gtk_widget_get_style_context(canvas);
	width = gtk_widget_get_allocated_width(canvas);
	height = gtk_widget_get_allocated_height(canvas);

	// Draw the weird, grayish background
	gtk_render_background(context, cr, 0, 0, width, height);

	// Draw each pixel, iterating over image size
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int c[COLOR_CHANNELS];
			for (int k = 0; k < COLOR_CHANNELS; k++) {
				c[k] = hex_to_decimal(((*image_ptr).data)[i][j][k]);
			}
			render_pixel(canvas, cr, i, j, c[0], c[1], c[2], c[3]);
		}
	}

	return FALSE;

}

void kill_app(GtkApplication *app, gpointer user_data){
	g_application_quit (app);
}

void activate(GtkApplication *app, gpointer user_data) {

	GtkWidget *windowTool;
	GtkWidget *image;
	GdkPixbuf *pixbuf;
	GtkWidget *window;
	GtkGrid *grid;
	GtkWidget *button;
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
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(kill_app), window);

	/* Place the Quit button in the grid cell (0, 1), and make it
	* span 2 columns.
	*/
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

	/* Create canvas as drawing_area object, and set size */
	canvas = gtk_drawing_area_new();
	gtk_widget_set_events(canvas, gtk_widget_get_events(canvas) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	gtk_widget_set_size_request(canvas, image_ptr->width, image_ptr->height);
	gtk_grid_attach(GTK_GRID(grid), canvas, 2, 2, 1, 1);
	g_signal_connect(G_OBJECT(canvas), "draw",
									G_CALLBACK(update_canvas), NULL);

	gtk_widget_show_all(window);

	//TOOLBAR Window
	/* create a new windowTool, and set its title */
	windowTool = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(windowTool), "windowTool");

	gtk_window_set_resizable(GTK_WINDOW(windowTool), FALSE);

	gtk_container_set_border_width(GTK_CONTAINER (windowTool), 10);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous(grid, 1);
	gtk_grid_set_column_homogeneous(grid, 1);

	/* Pack the container in the windowTool */
	gtk_container_add (GTK_CONTAINER (windowTool), grid);

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
	g_signal_connect(button, "clicked", G_CALLBACK(pencil), canvas);
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
	g_signal_connect(button, "clicked", G_CALLBACK(bucket), canvas);
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

	//Line Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(line), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/line.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	gtk_widget_show_all(windowTool);
}

int main(int argc, char **argv) {
	int status;
	int status2;
	struct Image image;
	image_ptr = &image;

	image = initialize_image(400, 400);

	GtkApplication *app;
	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);

	save_pdc(image, "wow.pdc");

	return status;
}
