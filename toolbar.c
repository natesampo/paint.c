#include <gtk/gtk.h>
#include "toolbar.h"
#include "paint.h"


//Button functions
void selecter(GtkWidget *widget, gpointer datal) {
	g_print ("Selection tool\n");
}

void move(GtkWidget *widget, gpointer data) {
	g_print ("Move tool\n");
}

void pencil(GtkWidget *widget, gpointer data) {
	g_print ("Pencil tool\n");
	GtkWidget* canvas = data;

	if (tool > 0) {
		g_signal_handler_disconnect(canvas, tool);
	}

	lastX = -1;
	lastY = -1;

	tool = g_signal_connect(canvas, "motion-notify-event", G_CALLBACK(brush_mouse_motion), NULL);
	printf("%d\n", tool);
}

void eraser(GtkWidget *widget, gpointer data) {
	g_print ("Eraser tool\n");
}

void text(GtkWidget *widget, gpointer data) {
	g_print ("Text tool\n");
}

void bucket(GtkWidget *widget, gpointer data) {
	g_print ("Bucket tool\n");
	GtkWidget* canvas = data;

	if (tool > 0) {
		g_signal_handler_disconnect(canvas, tool);
	}

	tool = g_signal_connect(canvas, "button-press-event", G_CALLBACK(paint_bucket_mouse_clicked), NULL);
	printf("%d\n", tool);
}

void picker(GtkWidget *widget, gpointer data) {
	GtkWidget *colorWheel;
	colorWheel = gtk_color_chooser_dialog_new ("Colors", NULL);
	gtk_dialog_run(colorWheel);

	gtk_color_chooser_get_rgba(colorWheel, &color);
	curr_color.red = (int)(color.red*255);
	curr_color.green = (int)(color.green*255);
	curr_color.blue = (int)(color.blue*255);
	curr_color.alpha = (int)(color.alpha*255);
	gtk_widget_destroy(colorWheel);
}

void line(GtkWidget *widget, gpointer data) {
	g_print ("Line tool\n");
}
