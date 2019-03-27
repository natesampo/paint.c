#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "paint.h"
#include "toolbar.h"

#define COLOR_CHANNELS 4
#define CHAR_DEPTH 2

// Initialize pointer to image
struct Image* image_ptr;

// Initialize brush object.
Brush* brush_ptr;

int tool = 0;
GdkRGBA color;
struct byteColor curr_color;
GtkWidget *canvas;

int lastX = -1;
int lastY = -1;

int last_sat = 0;

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

/* Given two integers with integer weights, returns the weighted average. */
double weighted_avg(double a, double a_wgt, double b, double b_wgt) {

	double total = a_wgt + b_wgt;
	if (total == 0) return 0;

	return (a * 1.0 * a_wgt/total + b * 1.0 * b_wgt/total);

}

/*
* Updates one pixel in the image with the values passed

* image - image structure to perform updates on
* x - x location of pixel in image
* y - y location of pixel in image
* r - red value of pixel to be rendered
* g - green value of pixel to be rendered
* b - blue value of pixel to be rendered
* a - alpha value of pixel to be rendered
*/

void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a) {
	if (check_bounds(image, x, y)) {

		int old_red = hex_to_decimal(image.data[x][y][0]);
		int old_green = hex_to_decimal(image.data[x][y][1]);
		int old_blue = hex_to_decimal(image.data[x][y][2]);
		int old_alpha = hex_to_decimal(image.data[x][y][3]);

		int alpha_weight = (255 - a) * old_alpha/255;
		int new_red = weighted_avg(old_red, alpha_weight, r, a);
		int new_green = weighted_avg(old_green, alpha_weight, g, a);
		int new_blue = weighted_avg(old_blue, alpha_weight, b, a);
		int new_alpha = weighted_avg(old_alpha, old_alpha, 255.0, a);

		// Get colors from rgba inputs
		char *red = decimal_to_hex(new_red);
		char *green = decimal_to_hex(new_green);
		char *blue = decimal_to_hex(new_blue);
		char *alpha = decimal_to_hex(new_alpha);

		// Write data to image structure
		image.data[x][y][0][0] = red[0];
		image.data[x][y][0][1] = red[1];
		image.data[x][y][1][0] = green[0];
		image.data[x][y][1][1] = green[1];
		image.data[x][y][2][0] = blue[0];
		image.data[x][y][2][1] = blue[1];
		image.data[x][y][3][0] = alpha[0];
		image.data[x][y][3][1] = alpha[1];

		// Free mallocated pointers
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

/*
* Return distance between two pixels
*
* image - image structure to perform updates on
* x1 - x coordinate of the first point
* y1 - y coordinate of the first point
* x2 - x coordinate of the second point
* y2 - y coordinate of the second point
*/
double get_distance(int x1, int y1, int x2, int y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

/*
* Draw a line between two points on the specificed image
*
* image - image structure to perform updates on
* x1 - x coordinate of the first point
* y1 - y coordinate of the first point
* x2 - x coordinate of the second point
* y2 - y coordinate of the second point
*/
void draw_line(struct Image image, int x1, int y1, int x2, int y2) {
	double distance = get_distance(x1, y1, x2, y2);
	int i;
	int cx;
	int cy;

	for(i=0; i<distance; i++) {
		cx = x1 - (int) ((x1 - x2)*(i/distance));
		cy = y1 - (int) ((y1 - y2)*(i/distance));
		draw_circle(image, cx, cy, brush_ptr -> size, brush_ptr -> hardness);
	}
}

void paint_bucket_recurse(struct byteColor *temp_color, int **visited, int x, int y) {

	if (check_bounds(*image_ptr, x, y) &&
		!visited[x][y] &&
		hex_to_decimal(image_ptr->data[x][y][0]) == temp_color->red &&
		hex_to_decimal(image_ptr->data[x][y][1]) == temp_color->green &&
		hex_to_decimal(image_ptr->data[x][y][2]) == temp_color->blue &&
		hex_to_decimal(image_ptr->data[x][y][3]) == temp_color->alpha) {

		visited[x][y] = 1;
		update_pixel(*image_ptr, x, y, curr_color.red, curr_color.green, curr_color.blue, curr_color.alpha);

		paint_bucket_recurse(temp_color, visited, x+1, y);
		paint_bucket_recurse(temp_color, visited, x, y+1);
		paint_bucket_recurse(temp_color, visited, x-1, y);
		paint_bucket_recurse(temp_color, visited, x, y-1);
	}
}

void paint_bucket_mouse_clicked(GtkWidget *widget, GdkEventButton *event, gpointer data) {

	if (event->button == 1 && check_bounds(*image_ptr, (int) event->x, (int) event->y)) {

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

		paint_bucket_recurse(&temp_color, visited, (int) event->x, (int) event->y);

		for (i=0; i<image_ptr->width; i++) {
			free(visited[i]);
		}
		free(visited);
	}
}

void line_mouse_released(GtkWidget *widget, GdkEventButton *event, gpointer data) {

	if (event->button == 1) {
		draw_line(*image_ptr, lastX, lastY, event->x, event->y);
		g_signal_handler_disconnect(canvas, tool);
		tool = g_signal_connect(canvas, "button-press-event", G_CALLBACK(line_mouse_clicked), NULL);
	}
}

void line_mouse_clicked(GtkWidget *widget, GdkEventButton *event, gpointer data) {

	if (event->button == 1) {
		lastX = (int) event->x;
		lastY = (int) event->y;
		g_signal_handler_disconnect(canvas, tool);
		tool = g_signal_connect(canvas, "button-press-event", G_CALLBACK(line_mouse_released), NULL);
	}
}

/* Draws a circle to the image structure at position (x, y) of diameter d.
	 Uses the color defined by curr_color.
*/
void draw_circle(struct Image image, int x, int y, int d, int hardness) {

	int min_x = x - d/2;
	int min_y = y - d/2;
	int max_x = x + d/2;
	int max_y = y + d/2;
	int dist;

	for (int this_x = min_x; this_x <= max_x; this_x++) {
		for (int this_y = min_y; this_y <= max_y; this_y++) {

			dist = get_distance(this_x, this_y, x, y);
			if (dist <= d/2) {
				double amt_0 = 2.0*dist/d - (brush_ptr-> hardness / 100.1);
				double amt_1 = 1.0 - 2.0*dist/d;
				double hardness_mult = MAX(0, MIN(1, weighted_avg(1.0, amt_1, 0.0, amt_0)));
				update_pixel(image, this_x, this_y,
					curr_color.red,
					curr_color.green,
					curr_color.blue,
					(int)(curr_color.alpha * hardness_mult));
			}
		}
	}

}

void brush_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data) {

	if (event->state & GDK_BUTTON1_MASK) {
		draw_line(*image_ptr, lastX, lastY, event->x, event->y);
	}

	lastX = event->x;
	lastY = event->y;
}

void free_image(struct Image image) {
	int i, j, k, width, height;

	image.width = width;
	image.height = height;

	for (i=0; i<image.width; i++) {
		for (j=0; j<image.width; j++) {
			for (k=0; k<COLOR_CHANNELS; k++) {
				free(image.data[i][j][k]);
			}

			free(image.data[i][j]);
		}

		free(image.data[i]);
	}

	free(image.data);
}

/* Load an image from a pdc file */
void load_pdc(GtkWidget *widget, gpointer data) {
	g_print ("Load tool\n");

	int input_char;
	int width = 0, height = 0, i = 0, j = 0, k = 0, l = 0;
	struct Image *image = malloc(sizeof(struct Image));
	FILE *input_file;
	char *file_name = "your_file.pdc";

	input_file = fopen(file_name, "r");

	// Read width from PDC file header
	while((input_char = getc(input_file)) != '\n') {
		width = 10*width + input_char - 48;
	}

	// Read height from PDC file header
	while((input_char = getc(input_file)) != '\n') {
		height = 10*height + input_char - 48;
	}

	image->width = width;
	image->height = height;

	// Allocate initial character arrays
	image->data = (char ****) malloc(image->width * sizeof(char ***));
	image->data[0] = (char ***) malloc(image->height * sizeof(char **));
	image->data[0][0] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
	image->data[0][0][0] = (char *) malloc(CHAR_DEPTH * sizeof(char));

	// Populate entire image array by reading one character at a time from the pdc file
	while((input_char = getc(input_file)) != EOF) {
		image->data[i][j][k][l] = input_char;

		// If we reach the character depth limit, move on to next color channel
		if (l == CHAR_DEPTH - 1) {
			l = 0;
			k++;
			image->data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		} else {
			l++;
		}

		// If we reach the last color channel, move on to the next column
		if (k == COLOR_CHANNELS) {
			k = 0;
			j++;
			image->data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
			image->data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		}

		// If we reach the end of a column, move on to the next row
		if (j == image->height) {
			j = 0;
			i++;
			image->data[i] = (char ***) malloc(image->height * sizeof(char **));
			image->data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
			image->data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		}
	}

	fclose(input_file);

	puts("done");
	free_image(*image_ptr);
	image_ptr = image;

	gtk_widget_queue_draw_area(canvas, 0, 0, image->width, image->height);
}

/*
	Save the image argument as the file name in pdc format

	PDC format:
	WIDTH\n
	HEIGHT\n
	DATA

	NOTE: There are no seperations in the data of the image file, ie no tabs, spaces, or newlines
*/
void save_pdc(GtkWidget *widget, gpointer data) {
	g_print ("Save tool\n");
	int i, j, k, l;
	FILE *output_file;
	char *file_name = "your_file.pdc";

	output_file = fopen(file_name, "w");

	fprintf(output_file, "%d", image_ptr->width);
	fputs("\n", output_file);
	fprintf(output_file, "%d", image_ptr->height);
	fputs("\n", output_file);

	for (i=0; i<image_ptr->width; i++) {

		for (j=0; j<image_ptr->height; j++) {

			for (k=0; k<COLOR_CHANNELS; k++) {

				for (l=0; l<CHAR_DEPTH; l++) {

					fprintf(output_file, "%c", image_ptr->data[i][j][k][l]);

				}
			}
		}
	}

	fclose(output_file);
}

/*
* Create and allocate memory for a filled image of the specified width and height
*
* width - width of image in pixels
* height - height of image in pixels
*/
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

/*
* Draw a single RGBA pixel on the canvas at position x, y
*
*
* GTK cairo array to store color data
* x - x location of pixel in image
* y - y location of pixel in image
* r - red value of pixel to be rendered
* g - green value of pixel to be rendered
* b - blue value of pixel to be rendered
* a - alpha value of pixel to be rendered
*/
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

/*
* Draws the pixels of the image onto the window to display the image.
*
* GTK cairo array to store color data
* data - values passed as gpointers, standard GTK function parameter
*/
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


/*
* End all GTK related functions for a given application
*/

void kill_app(GtkApplication *app, gpointer user_data){
	g_application_quit (app);
}


/*
* Change the saturation of every pixel in the image
*
*
* TODO Make the offset not wrap, actually make sense. Do research into how actual saturation algorithims work
*/
void edit_saturation(gdouble pos){
	int i,j;
	double r,g,b,a;
	guint width, height;
	width = gtk_widget_get_allocated_width(canvas);
	height = gtk_widget_get_allocated_height(canvas);



	// Update each pixel's saturation
	for (i=0; i<width; i++) {
		for (j=0; j<height; j++) {

			r = hex_to_decimal(image_ptr->data[i][j][0])*(pos-50);
			g = hex_to_decimal(image_ptr->data[i][j][1])+(pos-50);
			b = hex_to_decimal(image_ptr->data[i][j][2])+(pos-50);
			a = hex_to_decimal(image_ptr->data[i][j][3])+(pos-50);
			if(r>255){
				r = 255;
			}
			if(g>255){
				g = 255;
			}
			if(b>255){
				b = 255;
			}
			if(r<0){
				r = 0;
			}
			if(g<0){
				g = 0;
			}
			if(b<0){
				b = 0;
			}

			update_pixel(*image_ptr, i, j, r, g, b, a);
		}
	}

}

/*
* Callback function to be performed upon movement of the saturation slider
*
*/
void scale_moved (GtkRange *range, gpointer  user_data){
   GtkWidget *mylabel = user_data;
   gdouble pos = gtk_range_get_value (range);
   edit_saturation(pos);
   g_print("%f\n",pos);
}

void set_last_mouse_point(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
	lastX = (int) event->x;
	lastY = (int) event->y;
}

/*
* Callback function to be performed upon movement of the brush size slider
*
*/
void brush_slider_moved (GtkRange *range, gpointer  user_data){
   GtkWidget *mylabel = user_data;
   gdouble pos = gtk_range_get_value (range);
   brush_ptr -> size = pos;
   g_print("%f\n",pos);
}

/*
* Callback function to be performed upon movement of the hardness slider
*
*/
void hardness_slider_moved (GtkRange *range, gpointer  user_data){
   GtkWidget *mylabel = user_data;
   gdouble pos = gtk_range_get_value (range);
   brush_ptr -> hardness = pos;
   g_print("%f\n",pos);
}

void clear(GtkWidget *widget, gpointer data) {

	for (int x = 0; x < image_ptr -> width; x++) {
		for (int y = 0; y < image_ptr -> height; y++) {
			update_pixel(*image_ptr, x, y, 255, 255, 255, 255);
		}
	}

}

/*
* Begin the application window
*
* Like other GTK functions, app is the application running
* user_data is pointers to values to be used in the corresponding functions
*/
void activate(GtkApplication *app, gpointer user_data) {

	GtkWidget *windowTool;
	GtkWidget *label;
	GtkWidget *image;
	GdkPixbuf *pixbuf;
	GtkWidget *window;
	GtkGrid *grid;
	GtkWidget *button;
	GtkWidget *canvas_grid;
	GtkWidget *scale;
	GtkAdjustment *adjustment;

	/* create a new window, and set its title */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Canvas");
	gtk_window_set_default_size(GTK_WINDOW(window),
		image_ptr -> width,
		image_ptr -> height);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();

	/* Pack the container in the window */
	gtk_container_add(GTK_CONTAINER(window), grid);

	/* Create canvas as drawing_area object, and set size */
	canvas = gtk_drawing_area_new();
	gtk_widget_set_events(canvas, gtk_widget_get_events(canvas) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
	gtk_widget_set_size_request(canvas,
		image_ptr -> width,
		image_ptr -> height);
	gtk_grid_attach(GTK_GRID(grid), canvas, 2, 2, 1, 1);
	g_signal_connect(G_OBJECT(canvas), "draw",
									G_CALLBACK(update_canvas), NULL);

	gtk_widget_show_all(window);

	//TOOLBAR Window
	/* create a new windowTool, and set its title */
	windowTool = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(windowTool), "Toolbar");

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
	//gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/point.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Move Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(move), NULL);
	//gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/point.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Save Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(save_pdc), canvas);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/save.jpg", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Load Tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(load_pdc), canvas);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/load.png", 50, 50, 0, NULL);
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

	//Clear tool
	button = gtk_button_new();
	g_signal_connect(button, "clicked", G_CALLBACK(clear), canvas);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/sheet.png", 50, 50, 0, NULL);
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
	g_signal_connect(button, "clicked", G_CALLBACK(line), canvas);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1);
	pixbuf = gdk_pixbuf_new_from_file_at_scale("icons/line.png", 50, 50, 0, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_button_set_image (GTK_BUTTON (button), image);

	//Saturation Slider
	adjustment = gtk_adjustment_new (50, 0, 100, 5, 10, 0);
	label = gtk_label_new ("Saturation");
	scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adjustment);
  	gtk_scale_set_digits (GTK_SCALE (scale), 0);
  	gtk_grid_attach (GTK_GRID (grid), scale, 0, 6, 2, 1);
  	 gtk_grid_attach (GTK_GRID (grid), label, 0, 5, 2, 1);
  	gtk_widget_set_hexpand (scale, TRUE);
  	gtk_widget_set_valign (scale, GTK_ALIGN_START);
  	g_signal_connect (scale,
                    "value-changed",
                    G_CALLBACK (scale_moved),
                    label);

		//Brush Size Slider
		adjustment = gtk_adjustment_new (8, 1, 50, 5, 10, 0);
		label = gtk_label_new ("Brush Size");
		scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adjustment);
	  	gtk_scale_set_digits (GTK_SCALE (scale), 0);
	  	gtk_grid_attach (GTK_GRID (grid), scale, 0, 7, 2, 1);
	  	gtk_grid_attach (GTK_GRID (grid), label, 0, 6, 2, 1);
	  	gtk_widget_set_hexpand (scale, TRUE);
	  	gtk_widget_set_valign (scale, GTK_ALIGN_START);
	  	g_signal_connect (scale,
	                    "value-changed",
	                    G_CALLBACK (brush_slider_moved),
	                    label);

		//Hardness Slider
		adjustment = gtk_adjustment_new (100, 0, 100, 5, 10, 0);
		label = gtk_label_new ("Brush Hardness");
		scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adjustment);
	  	gtk_scale_set_digits (GTK_SCALE (scale), 0);
	  	gtk_grid_attach (GTK_GRID (grid), scale, 0, 9, 2, 1);
	  	gtk_grid_attach (GTK_GRID (grid), label, 0, 8, 2, 1);
	  	gtk_widget_set_hexpand (scale, TRUE);
	  	gtk_widget_set_valign (scale, GTK_ALIGN_START);
	  	g_signal_connect (scale,
	                    "value-changed",
	                    G_CALLBACK (hardness_slider_moved),
	                    label);


	gtk_widget_show_all(windowTool);
}

/* Initialize a new brush and return a pointer.
*/
Brush* new_brush(int size, int hardness) {

	Brush* new_ptr = malloc(sizeof(Brush));
	new_ptr -> size = size;
	new_ptr -> hardness = hardness;
	return new_ptr;

}

int main(int argc, char **argv) {
	int status;
	int status2;
	struct Image image;
	image_ptr = &image;
	brush_ptr = new_brush(6, 100);

	image = initialize_image(400, 400);

	GtkApplication *app;
	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);

	return status;
}
