/*
* Paint functions header file
*
*/

struct Image {	int width;
				int height;
				char ****data;
			};

/* Brush structure */
typedef struct {
				int size;			//	Diameter of circular brush, in pixels
				int hardness;	//	Hardness of brush, as integer from 0 to 100, where 0 is softest
			} Brush;

int hex_to_decimal(char* hex);
char* decimal_to_hex(int n);

void update_pixel(struct Image image, int x, int y, int r, int g, int b, int a);
void draw_circle(struct Image image, int x, int y, int d, int hardness);

extern int tool;

Brush* new_brush(int size, int hardness);
void load_pdc(GtkWidget *widget, gpointer data);
void save_pdc(GtkWidget *widget, gpointer data);
struct Image initialize_image(int width, int height);
gboolean update_canvas(GtkWidget* canvas, cairo_t *cr, gpointer data);
void activate(GtkApplication *app, gpointer user_data);
void brush_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data);
void eraser_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data);
void paint_bucket_mouse_clicked(GtkWidget *widget, GdkEventButton *event, gpointer data);
void line_mouse_released(GtkWidget *widget, GdkEventButton *event, gpointer data);
void line_mouse_clicked(GtkWidget *widget, GdkEventButton *event, gpointer data);