#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include "sokoban.h"
#include "mouvement.h"

typedef enum
{
    S_HAUT, S_BAS, S_GAUCHE, S_DROITE, S_JOUEUR_CIBLE, S_MUR, S_CIBLE, S_CAISSE, S_CAISSE_CIBLE, S_SOL
} Sprites;

char initSDL(SDL_Surface** ecran);
void freeSDL(SDL_Surface*** sprites);

void loadSprites(SDL_Surface*** sprites);
void libereSprites(SDL_Surface*** sprites);
void dessineNiveau(Niveau* n, SDL_Surface* fond, int largeur, int hauteur, SDL_Surface*** sprites);
void effacerNiveau(SDL_Surface* fond);
Sprites choixSprite(char objet);
char clavierHandler(SDL_Event* event, Niveau* n, Position* posJoueur, CoupsJoues* pile, int* nbMvt, int* nbPoussee);


#endif
