/**
 * \file mouvement.h
 * \brief (Prototypes) Fonctions relatives aux deplacements.
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include <stdlib.h>
#include <stdio.h>
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

PileError empilerCoup(CoupsJoues* pile, Direction direction, Position posDepart, char caisseDeplacee);
Coup* depilerCoup(CoupsJoues* pile);
void libererPile(CoupsJoues* pile);
PileError inverserPileCoups(CoupsJoues* pile, CoupsJoues* pileInversee);

void deplacerObjet(Niveau* n, Coup mouvement, unsigned int* nbMvt, unsigned int* nbPoussee);
void deplacerObjetRetour(Niveau* n, Coup mouvement, unsigned int* nbMvt, unsigned int* nbPoussee);

void majNiveau(Position positionDepart, Position position, Niveau* n);

TypeDeplacement deplacementPossible(Niveau* n, Position position, char liberte, Direction direction);

void calculPosition(Position* position, Direction direction, SensDeplacement sens);

void arrowKeyDownHandler(Direction dir, Niveau* n, Position* posJoueur, CoupsJoues* pile, unsigned int* nbMvt, unsigned int* nbPoussee);
#endif
