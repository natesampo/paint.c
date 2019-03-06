#include <gtk/gtk.h>
#include "toolbar.h"
#include "paint.h"


//Button functions
void selecter(GtkWidget *widget, gpointer datal){
	g_print ("Selection tool\n");
	tool = 1;

}
void move(GtkWidget *widget, gpointer data){
	g_print ("Move tool\n");
	tool = 2;
}

void pencil(GtkWidget *widget, gpointer data){
	g_print ("Pencil tool\n");
	tool = 3;
}

void eraser(GtkWidget *widget, gpointer data){
	g_print ("Eraser tool\n");
	tool = 4;
}

void text(GtkWidget *widget, gpointer data){
	g_print ("Text tool\n");
	tool = 5;
}

void bucket(GtkWidget *widget, gpointer data){
	g_print ("Paint bucket tool\n");
	tool = 6;
}

void picker(GtkWidget *widget, gpointer data){
	GtkWidget *colorWheel;
	colorWheel = gtk_color_chooser_dialog_new ("Colors", NULL);
	int response = gtk_dialog_run(colorWheel);

	gtk_color_chooser_get_rgba(colorWheel, &color);
	curr_color.red = (int)(color.red*255);
	curr_color.green= (int)(color.green*255);
	curr_color.blue= (int)(color.blue*255);
	curr_color.alpha= (int)(color.alpha*255);
	gtk_widget_destroy(colorWheel);

	tool = 7;

}

