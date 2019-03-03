paint: toolbox.o paint.o toolbar.o
	gcc -Wall  toolbox.o paint.o toolbar.o -o paint

toolbox.o: toolbox.c toolbox.h
	gcc -Wall -c toolbox.c

paint.o: paint.c paint.h
	gcc -Wall -c paint.c

toolbar.o: toolbar.c tollbar.h
	gcc -Wall -c toolbar.c

