/**
 * \file level.h
 * \brief (Prototypes) Fonctions utiles a la lecture d'un niveau
 * \author Dorialex
 * \version 0.1
 * \date 16 mai 2013
 *
 * Prototypes des fonctions du fichier level.c, regroupant toutes les fonctions utiles a la lecture d'un niveau.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <stdio.h>
#include "sokoban.h"

typedef enum {
	NoError,
	FichierIntrouvable,
	AllocationMemoire,
	CaractereInconnu
} LevelError;


void getTailleNiveau(FILE* niveaux, int numero, int* largeur, int* hauteur, fpos_t* positionNiveau);

LevelError alloueNiveau(Niveau* niveau, int largeur, int hauteur);
void freeNiveau(Niveau* n);
LevelError remplirNiveau(FILE* levels, fpos_t* position, Niveau* n, int w, int h, Position* posJoueur);
LevelError caractereValide(Niveau* niveau, char* ligne, int numeroLigne, int largeur, Position* posJoueur);

LevelError readLevel(char* path, Niveau* niveau, int numero, int* largeurNiveau, int* hauteurNiveau, Position* posJoueur);

#endif
