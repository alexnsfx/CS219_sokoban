OUT = sokoban.out
CFLAGS = -g -Wall -lm `sdl-config --cflags` #-I./include 
LFLAGS = `sdl-config --libs` -ldl -lm #-L./lib -lfmodex

all: main.o level.o mouvement.o graphic.o
	gcc main.o level.o mouvement.o graphic.o -o $(OUT) $(LFLAGS) 

main.o:
	gcc -c $(CFLAGS) main.c -o main.o
	
level.o:
	gcc -c $(CFLAGS) level.c -o level.o
	
mouvement.o:
	gcc -c $(CFLAGS) mouvement.c -o mouvement.o

graphic.o:
	gcc -c $(CFLAGS) graphic.c -o graphic.o

clean:
	rm *.o
	rm $(OUT)
