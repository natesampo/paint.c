/*
* This will be the header file for toolbar functions
*
*/

struct byteColor{
	int red;
	int green;
	int blue;
	int alpha;
};

extern GdkRGBA color;
extern struct byteColor curr_color;
extern int tool;
extern int lastX;
extern int lastY;

void controller(GtkApplication *app, gpointer user_data);
void selecter(GtkWidget *widget, gpointer datal);
void move(GtkWidget *widget, gpointer data);
void pencil(GtkWidget *widget, gpointer data);
void eraser(GtkWidget *widget, gpointer data);
void text(GtkWidget *widget, gpointer data);
void bucket(GtkWidget *widget, gpointer data);
void picker(GtkWidget *widget, gpointer data);
void line(GtkWidget *widget, gpointer data);