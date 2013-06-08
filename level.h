/**
 * \file level.h
 * \brief (Prototypes) Fonctions utiles a la lecture d'un niveau
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 16 mai 2013
 *
 * Prototypes des fonctions du fichier level.c, regroupant toutes les fonctions utiles a la lecture d'un niveau.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sokoban.h"

typedef enum {
	NoError,
	UndefinedLevel,
	UndefinedParameter,
	FichierIntrouvable,
	AllocationMemoire,
	CaractereInconnu
} LevelError;

typedef enum {
	LevelReset,
	LevelPrevious,
	LevelNext
} UpdateType;

typedef enum {
	CibleNonValidee,
	CibleValidee
} ResultatCible;

typedef struct Cible Cible;
struct Cible {
	unsigned int x;
	unsigned int y;
	ResultatCible validee;
	Cible* next;
};

typedef Cible* PileCible;

ResultatCible toutesCiblesValidees(PileCible* pile, Niveau* niveau);
PileError empilerCible(PileCible* pile, unsigned int x, unsigned int y, char validee);
void libererPileCible(PileCible* pile);

void getTailleNiveau(FILE* niveaux, unsigned int numero, unsigned int* largeur, unsigned int* hauteur, fpos_t* positionNiveau);

LevelError alloueNiveau(Niveau* niveau, unsigned int largeur, unsigned int hauteur);
void freeNiveau(Niveau* n, unsigned int hauteur);
LevelError remplirNiveau(FILE* levels, fpos_t* position, Niveau* n, unsigned int w, unsigned int h, Position* posJoueur, PileCible* pileCible);
LevelError caractereValide(Niveau* niveau, char* ligne, unsigned int numeroLigne, unsigned int largeur, Position* posJoueur, PileCible* pileCible);

LevelError readLevel(char* path, Niveau* niveau, unsigned int numero, unsigned int* largeurNiveau, unsigned int* hauteurNiveau, Position* posJoueur, PileCible* pileCible);
LevelError updateLevel(UpdateType operation, char* fichierNiveaux, Niveau* niveau, Position* posJoueur, PileCible* pileCible, unsigned int* numLvl, unsigned int* hNiveau, unsigned int* lNiveau);
LevelError createLevel(Niveau* niveau, unsigned int* largeurNiveau, unsigned int* hauteurNiveau);

#endif
