#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define COLOR_CHANNELS 4
#define CHAR_DEPTH 2

struct Image {
	int width, height;
	char ****data;
};

static char* decimal_to_hex(int n) {
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

// NOTE: Assumes 4 color channel values
static void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a) {
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

static struct Image load_pdc(char *file_name) {
	int input_char;
	int width = 0, height = 0, i = 0, j = 0, k = 0, l = 0;
	struct Image image;
	FILE *input_file;

	input_file = fopen(file_name, "r");

	while((input_char = getc(input_file)) != '\n') {
		width = 10*width + input_char - 48;
	}

	while((input_char = getc(input_file)) != '\n') {
		height = 10*height + input_char - 48;
	}

	image.width = width;
	image.height = height;

	image.data = (char ****) malloc(image.width * sizeof(char ***));
	image.data[0] = (char ***) malloc(image.height * sizeof(char **));
	image.data[0][0] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
	image.data[0][0][0] = (char *) malloc(CHAR_DEPTH * sizeof(char));

	while((input_char = getc(input_file)) != EOF) {
		image.data[i][j][k][l] = input_char;

		if (l == CHAR_DEPTH - 1) {
			l = 0;
			k++;
			image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		} else {
			l++;
		}

		if (k == COLOR_CHANNELS) {
			k = 0;
			j++;
			image.data[i][j] = (char **) malloc(COLOR_CHANNELS * sizeof(char *));
			image.data[i][j][k] = (char *) malloc(CHAR_DEPTH * sizeof(char));
		}

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

static void save_pdc(struct Image image, char *file_name) {
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

static struct Image initialize_image(int width, int height) {
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

					image.data[i][j][k][l] = 'f';

				}
			}
		}
	}

	return image;
}

static void print_hello(GtkWidget *widget, gpointer data) {
	g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
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

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	struct Image image;

	image = initialize_image(500, 500);

	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);

	save_pdc(image, "wow.pdc");

	return status;
}