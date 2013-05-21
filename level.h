#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <stdio.h>

char getTailleNiveau(FILE* niveaux, int* largeur, int* hauteur);

char** readLevel(char* path, int numero);


#endif