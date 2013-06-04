#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include <stdlib.h>
#include "sokoban.h"

typedef enum {
    HAUT, BAS, GAUCHE, DROITE
} Direction;

typedef struct Coup Coup;
struct Coup
{
	Direction direction;
	Position posDepart;
	char caisseDeplacee; /* 1 si caisse a deplacer */
	Coup* next;
};

typedef Coup* CoupsJoues;

typedef enum {
	Possible,
	PossibleAvecCaisse,
	Impossible
} TypeDeplacement;

typedef enum {
	Positif = 1,
	Negatif = -1
} SensDeplacement;

typedef enum {
	Ok, 
	AllocError, 
	UndefinedPile
} PileError;

PileError empilerCoup(CoupsJoues* pile, Direction direction, Position posDepart, char caisseDeplacee);
Coup* depilerCoup(CoupsJoues* pile);
void libererPile(CoupsJoues* pile);

void deplacerObjet(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee);
void deplacerObjetRetour(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee);

void majNiveau(Position positionDepart, Position position, Niveau* n);

void annulerCoup(Niveau* n, CoupsJoues* pile, int* nbMvt, int* nbPoussee);

TypeDeplacement deplacementPossible(Niveau* n, Position position, int liberte, Direction direction);

void calculPosition(Position* position, Direction direction, SensDeplacement sens);

#endif
