#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include <stdlib.h>
#include "sokoban.h"

typedef enum
{
    HAUT, BAS, GAUCHE, DROITE
} Direction;

typedef struct Coup Coup;
struct Coup
{
	Direction direction;
	int posDepartOrdonnee;
	int posDepartAbscisse;
	char caisseDeplacee; /* 1 si caisse a deplacer*/
	Coup* next;
};

typedef Coup* CoupsJoues;

char empilerCoup(CoupsJoues* pile, Direction direction, int posDepartAbscisseint, int posDepartOrdonnee, char caisseDeplacee);
Coup* depilerCoup(CoupsJoues* pile);
void libererPile(CoupsJoues* pile);

void deplacerObjet(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee);
void deplacerObjetRetour(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee);

void annulerCoup(Niveau* n, CoupsJoues* pile, int* nbMvt, int* nbPoussee);

char deplacementPossible(Niveau* n, int ordonnee, int abscisse, int liberte, Direction direction);

void majNiveau(int ordonneeInit, int abscisseInit, int ordonnee, int abscisse, Niveau* n);

void calculPositionPositif (int* ordonnee, int* abscisse, Coup* mouvement);
void calculPositionNegatif (int* ordonnee, int* abscisse, Coup* mouvement);

#endif