OUT = sokoban.out
CFLAGS = -g -Wall #-I./include 
LFLAGS = #-L./lib -lfmodex

all: main.o level.o mouvement.o
	gcc main.o level.o mouvement.o -o $(OUT) $(LFLAGS) 

main.o:
	gcc -c $(CFLAGS) main.c -o main.o
	
level.o:
	gcc -c $(CFLAGS) level.c -o level.o
	
mouvement.o:
	gcc -c $(CFLAGS) mouvement.c -o mouvement.o
	
clean:
	rm *.o
	rm $(OUT)
