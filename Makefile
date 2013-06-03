OUT = sokoban.out
CFLAGS = -g -Wall
LFLAGS = 

all: main.o level.o
	gcc main.o level.o -o $(OUT) $(LFLAGS) 

main.o:
	gcc -c $(CFLAGS) main.c -o main.o
	
level.o:
	gcc -c $(CFLAGS) level.c -o level.o
	
clean:
	rm *.o
	rm $(OUT)
