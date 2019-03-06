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
	curr_color.red = color.red*255;
	curr_color.green= color.green*255;
	curr_color.blue= color.blue*255;
	curr_color.alpha= color.alpha*255;
	gtk_widget_destroy(colorWheel);

	g_print("red %f\n", color.red);
	g_print("green %f\n", color.green);
	
	g_print("blue %f\n", color.blue);
	g_print("alpha %f\n", color.alpha);

	g_print("After select:\n");
	g_print("red %f\n", curr_color.red);
	g_print("green %f\n", curr_color.green);
	g_print("blue %f\n", curr_color.blue);
	g_print("alpha %f\n", curr_color.alpha);

	tool = 7;

}

