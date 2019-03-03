/*
* Paint functions header file
*
*/

int hex2d(char* hex);
static char* decimal_to_hex(int n);
static void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a);
static struct Image load_pdc(char *file_name);
static void save_pdc(struct Image image, char *file_name);
static struct Image initialize_image(int width, int height);
static void print_hello(GtkWidget *widget, gpointer data); 
gboolean update_canvas(GtkWidget* canvas, cairo_t *cr, gpointer data);
static void activate(GtkApplication *app, gpointer user_data);
struct Image {	int width;
				int height; 
				char ****data;
			};
