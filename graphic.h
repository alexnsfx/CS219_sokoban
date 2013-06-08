/**
 * \file graphic.h
 * \brief (Prototypes) Fonctions relatives a la SDL. Toutes les fonctions de dessin.
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "sokoban.h"

typedef enum {
	S_MUR, 
	S_CIBLE,
	S_CAISSE, 
	S_CAISSE_CIBLE, 
	S_JOUEUR_CIBLE, 
	S_BAS, 
	S_HAUT, 
	S_GAUCHE, 
	S_DROITE, 
	S_SOL
} SpriteType;

typedef enum {
	InitOk, 
	InitError
} SDLInitError;

SDLInitError initSDL(SDL_Surface** ecran);
void freeSDL(SDL_Surface*** sprites);

SDLInitError loadSprites(SDL_Surface*** sprites);
void libereSprites(SDL_Surface*** sprites);

void dessineEntete(SDL_Surface* fond);
void dessineCompteurs(SDL_Surface* fond, unsigned int nbMvt, unsigned int nbPoussee);
void dessinerDescriptif(SDL_Surface* fond, SpriteType objetADessiner, SDL_Surface*** sprites);

void dessinerElement(SpriteType objetADessiner, int abscisse, int ordonnee, SDL_Surface* fond, SDL_Surface*** sprites, Niveau* level, int w, int h);

void dessineNiveau(Niveau* n, SDL_Surface* fond, unsigned int largeur, unsigned int hauteur, SDL_Surface*** sprites);
void effacerNiveau(SDL_Surface* fond);

SpriteType choixSprite(char objet);
char choixCaractere(SpriteType sprite);

#endif
