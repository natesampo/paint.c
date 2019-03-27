/*
* Paint functions header file
*
*/

struct Image {	int width;
				int height;
				char ****data;
			};

typedef struct { int size;
				int hardness;
			} Brush;

int hex_to_decimal(char* hex);
char* decimal_to_hex(int n);

void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a);
void draw_circle(struct Image image, int x, int y, int d);

struct Image load_pdc(char *file_name);
void save_pdc(struct Image image, char *file_name);
struct Image initialize_image(int width, int height);
gboolean update_canvas(GtkWidget* canvas, cairo_t *cr, gpointer data);
void activate(GtkApplication *app, gpointer user_data);
void brush_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data);
void paint_bucket_mouse_clicked(GtkWidget *widget, GdkEventMotion *event, gpointer data);
