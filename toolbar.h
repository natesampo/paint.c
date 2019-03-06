/*
* This will be the header file for toolbar functions
*
*/

struct byteColor{
	double red;
	double green;
	double blue;
	double alpha;
};

extern GdkRGBA color;
extern struct byteColor curr_color;
extern int tool;

void controller(GtkApplication *app, gpointer user_data);
void selecter(GtkWidget *widget, gpointer datal);
void move(GtkWidget *widget, gpointer data);
void pencil(GtkWidget *widget, gpointer data);
void eraser(GtkWidget *widget, gpointer data);
void text(GtkWidget *widget, gpointer data);
void bucket(GtkWidget *widget, gpointer data);
void picker(GtkWidget *widget, gpointer data);