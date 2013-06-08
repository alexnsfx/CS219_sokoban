OUT = sokoban.out
EDITEUR = editeur.out
UNITAIRE = testunitaire.out
CFLAGS = -g -Wall -pedantic `sdl-config --cflags` -I./include 
LFLAGS = `sdl-config --libs` -ldl -lm -L./lib -lSDL_ttf -lfreetype

all: sokoban editeur testunitaire

sokoban: main.o level.o mouvement.o graphic.o save.o
	gcc main.o level.o mouvement.o graphic.o save.o -o $(OUT) $(LFLAGS) 

editeur: editeur.o level.o mouvement.o graphic.o save.o
	gcc editeur.o level.o mouvement.o graphic.o save.o -o $(EDITEUR) $(LFLAGS) 

testunitaire: testunitaire.o mouvement.o
	gcc testunitaire.o mouvement.o -o $(UNITAIRE) $(LFLAGS) 

main.o:
	gcc -c $(CFLAGS) main.c -o main.o

editeur.o:
	gcc -c $(CFLAGS) editeur.c -o editeur.o	
	
level.o:
	gcc -c $(CFLAGS) level.c -o level.o
	
mouvement.o:
	gcc -c $(CFLAGS) mouvement.c -o mouvement.o

graphic.o:
	gcc -c $(CFLAGS) graphic.c -o graphic.o

save.o:
	gcc -c $(CFLAGS) save.c -o save.o

testunitaire.o:
	gcc -c $(CFLAGS) testunitaire.c -o testunitaire.o

clean:
	rm *.o
	rm $(OUT)
	rm $(EDITEUR)
	rm $(UNITAIRE)

