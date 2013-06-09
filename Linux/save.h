/**
 * \file save.h
 * \brief (Prototypes) Fonctions relatives a la sauvegarde de la partie et des solutions.
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include "sokoban.h"
#include "level.h"
#include "mouvement.h"

typedef enum {
	Sauvegarde,
	Solution
} SaveType;

typedef enum {
	CreateOk,
	CreateError
} SaveFileError;

typedef enum {
	LoadOk,
	LoadError,
	LoadUndefinedParameter
} LoadFileError;

typedef enum {
	FichierNul,
	NiveauIntrouvable,
	NiveauPlace
} NiveauPosition;

typedef enum {
	EditionNoError,
	EditionUndefinedLevel,
	EditionUndefinedPath,
	EditionOpenFileError
} EditionError;

typedef enum {
	SaveLoadNoError,
	SaveLoadUndefinedPile,
	SaveLoadUndefinedFile,
	SaveLoadEmpileError
} PileSaveLoadError;

PileSaveLoadError chargerPileCoups(CoupsJoues* pile, FILE* fichier);
PileSaveLoadError dechargerPileCoups(CoupsJoues* pile, FILE* fichier);

SaveFileError creerFichierSauvegarde(SaveType sType, int numero, CoupsJoues* pile);
LoadFileError chargerFichierSauvegarde(unsigned int numero, CoupsJoues* pileCoups, Niveau* n, Position* pos, PileCible* cibles, unsigned int* nbMvt, unsigned int* nbPoussee);

EditionError editeNiveau(char* fichierLevel, char* fichierTemp, Niveau* niveau, int numLvl, int hauteur);

#endif
