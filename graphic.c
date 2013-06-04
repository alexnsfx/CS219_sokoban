#include "graphic.h"

SDLInitError initSDL(SDL_Surface** ecran) {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Erreur d'initialisation de la SDL ! %s\n", SDL_GetError());
		return Error;
	}
	SDL_WM_SetCaption(NOM_JEU, NULL);
	*ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, COLOR_BIT, SDL_HWSURFACE);
	if(*ecran == NULL) {
		fprintf(stderr, "Impossible de charger le mode video ! %s\n", SDL_GetError());
		return Error;
	}
	
	SDL_FillRect(*ecran, NULL, SDL_MapRGB((*ecran)->format, 0, 0, 0));
	return Ok;
}

void freeSDL(SDL_Surface*** sprites) {
	libereSprites(sprites);
	SDL_Quit();
}

/**
 * \fn void loadSprites(SDL_Surface** sprites[])
 * \brief Contient tous les sprites
 * \param SDL_Surface** sprites[] Pointeur sur le tableau de pointeurs de SDL surface
 */
void loadSprites(SDL_Surface*** sprites) {
	/* definition des images a afficher */
	(*sprites)[S_HAUT] = SDL_LoadBMP("sprites/HAUT.bmp");
	(*sprites)[S_BAS] = SDL_LoadBMP("sprites/BAS.bmp");
	(*sprites)[S_GAUCHE] = SDL_LoadBMP("sprites/GAUCHE.bmp");
	(*sprites)[S_DROITE] = SDL_LoadBMP("sprites/DROITE.bmp");
	(*sprites)[S_JOUEUR_CIBLE] = SDL_LoadBMP("sprites/BAS.bmp");
	(*sprites)[S_MUR] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, COLOR_BIT, 0, 0, 0, 0); 
	(*sprites)[S_CIBLE] = SDL_LoadBMP("sprites/CIBLE.bmp"); 
	(*sprites)[S_CAISSE] = SDL_LoadBMP("sprites/CAISSE.bmp"); 
	(*sprites)[S_CAISSE_CIBLE] = SDL_LoadBMP("sprites/CAISSE_CIBLE.bmp"); 
	(*sprites)[S_SOL] = SDL_LoadBMP("sprites/SOL.bmp"); 

	/* Choix des couleurs */
	SDL_FillRect((*sprites)[S_MUR], NULL, SDL_MapRGB((*sprites)[S_MUR]->format, 70, 70, 70));
}

void libereSprites(SDL_Surface*** sprites) {
	int i;	
	for(i = 0; i < NB_SPRITE; i++) {
		SDL_FreeSurface((*sprites)[i]);
	}
}

void dessineNiveau(Niveau* n, SDL_Surface* fond, int largeur, int hauteur, SDL_Surface*** sprites) {
	SDL_Rect pos;
	Sprites numeroSprite;
	int i, j;
	for(j = 0; j < hauteur; j++) {
		for(i = 0; i < largeur; i++) {
			pos.x = i * TAILLE_BLOC;
			pos.y = j * TAILLE_BLOC;
			numeroSprite = choixSprite((*n)[j][i]);
			SDL_BlitSurface((*sprites)[numeroSprite], NULL, fond, &pos);
		}
	}
}

void effacerNiveau(SDL_Surface* fond) {
	SDL_FillRect(fond, NULL, SDL_MapRGB(fond->format, 0, 0, 0));
}

SpriteType choixSprite(char objet) {
	switch(objet) {
		case JOUEUR:
			return S_BAS;
		case MUR:
			return S_MUR;
		case CIBLE:
			return S_CIBLE;
		case CAISSE:
			return S_CAISSE;
		case SOL:
			return S_SOL;
		case CAISSE_CIBLE:
			return S_CAISSE_CIBLE;
		case JOUEUR_CIBLE:
			return S_JOUEUR_CIBLE;
		default:
			return S_SOL;
	}
}

void keyDownHandler(Direction dir, Niveau* n, Position* posJoueur, CoupsJoues* pile, int* nbMvt, int* nbPoussee) {
	TypeDeplacement deplacementCaisse = Impossible;
	deplacementCaisse = deplacementPossible(n, (*posJoueur).y, (*posJoueur).x, 0, dir);
	if(deplacementCaisse < Impossible) {
		empilerCoup(pile, dir, (*posJoueur).y, (*posJoueur).x, deplacementCaisse);
		deplacerObjet(n, *pile, nbMvt, nbPoussee);
		calculPositionPositif(&((*posJoueur).y), &((*posJoueur).x), dir);
	}
}
