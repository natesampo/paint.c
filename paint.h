/*
* Paint functions header file
*
*/

int hex2d(char* hex);
char* decimal_to_hex(int n);

void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a);

struct Image load_pdc(char *file_name);
void save_pdc(struct Image image, char *file_name);
struct Image initialize_image(int width, int height);
gboolean update_canvas(GtkWidget* canvas, cairo_t *cr, gpointer data);
void activate(GtkApplication *app, gpointer user_data);
struct Image {	int width;
				int height; 
				char ****data;
			};
