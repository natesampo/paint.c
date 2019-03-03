paint: paint.o toolbar.o
	gcc -Wall paint.o toolbar.o -o paint `pkg-config --cflags --libs gtk+-3.0`


# paint: toolbox.o paint.o toolbar.o
# 	gcc -Wall  toolbox.o paint.o toolbar.o -o paint

# toolbox.o: toolbox.c toolbox.h
# 	gcc -Wall -c toolbox.c

paint.o: paint.c paint.h
	gcc -Wall -c paint.c `pkg-config --cflags --libs gtk+-3.0`

toolbar.o: toolbar.c toolbar.h
	gcc -Wall -c toolbar.c `pkg-config --cflags --libs gtk+-3.0`

