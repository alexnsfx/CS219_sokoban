/**
 * \file editeur.c
 * \brief Fichier principal de l'editeur de niveau.
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "sokoban.h"
#include "level.h"
#include "mouvement.h"
#include "graphic.h"
#include "save.h"

/**
 * \fn SpriteType ObjetSelect(int molette)
 * \brief Selectionne le bon sprite en fonction de la molette de la souris
 *
 * \param molette Nombre associe a la molette (+1 vers le haut -1 vers le bas)
 * \return Renvoie le sprite a utiliser
 */
SpriteType ObjetSelect(int molette) {
	int objet = molette % 6;
	while(objet < 0) {
		objet += 6;
	}
	return (SpriteType)objet;
}

/**
 * \fn void afficheNiveau(Niveau* n, int w, int h) 
 * \brief Affiche le niveau en console
 *
 * \param n Pointeur sur le tableau de niveau
 * \param w Largeur du tableau 
 * \param h Hauteur du tableau
 *
 * \return Renvoie le sprite a utiliser
 */
void afficheNiveau(Niveau* n, int w, int h) {
	int i = 0, j = 0;	
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			printf("%c", (*n)[i][j]);
		}
		printf("\n");
	}
}

/**
 * \fn int main(int argc, char* argv[]) 
 * \brief Fonction principale de l'editeur
 */
int main(int argc, char* argv[]) {
	EditionError edit_e;
	LevelError code_e;
	Niveau level;
	unsigned int largeurNiveau = 0, hauteurNiveau = 0;
	int numLvl = 1;
	Position positionJoueur;
	PileCible pile = NULL;

	SpriteType objetSelect = S_HAUT;
	char rEnfoncee = 0, lEnfoncee = 0;
	int molette = 0;  

	char continuerBoucle = 1;

	SDLInitError sdl_e;
	SDL_Surface *ecran;
	SDL_Event event;
	SDL_Surface** tableauSprites = NULL;
	
	if(argc > 1) {
		numLvl = atoi(argv[1]);	/* Si choix du niveau lors du lancement de l'editeur */
	}
	else {
		printf("Quel niveau voulez-vous editer ? \n");
		scanf("%d", &numLvl);
	}

	positionJoueur.x = 0;
	positionJoueur.y = 0;

	if(numLvl < 1 && numLvl > MAX_LEVEL) {	/* Si le niveau n'existe pas on charge le 1er */
		numLvl = 1;
	}

	code_e = readLevel(FICHIER_NIVEAUX, &level, numLvl, &largeurNiveau, &hauteurNiveau, &positionJoueur, &pile);
	if(code_e != NoError) {	/* Verification du chargement du niveau */
		freeNiveau(&level, hauteurNiveau);
		exit(EXIT_FAILURE);
	}
	libererPileCible(&pile);
	
	sdl_e = initSDL(&ecran);
	if(sdl_e != InitOk) {	/* Verification du lancement du mode graphique */
		fprintf(stderr, "(%d) Erreur lors de initSDL(SDL_Surface**).\n", sdl_e);
		freeNiveau(&level, hauteurNiveau);
		freeSDL(NULL);
		exit(EXIT_FAILURE);
	}
	
	tableauSprites = (SDL_Surface**)malloc(NB_SPRITE * sizeof(SDL_Surface*));
	if(tableauSprites == NULL) {	/* Verification de l'allocation du tableau des sprites */
		fprintf(stderr, "Erreur d'allocation du tableau de sprites.\n");
		freeNiveau(&level, hauteurNiveau);
		freeSDL(NULL);
		exit(EXIT_FAILURE);
	}
	
	sdl_e = loadSprites(&tableauSprites);
	if(sdl_e != InitOk) {	/* Verification du chargement des sprites */
		fprintf(stderr, "(%d) Erreur de chargement des sprites dans le tableau de sprites.\n", sdl_e);
		freeNiveau(&level, hauteurNiveau);
		freeSDL(&tableauSprites);
		exit(EXIT_FAILURE);
	}
	/* On dessine la partie statique du niveau */
	dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);
	dessineEntete(ecran);	

	while (continuerBoucle)
	{
		while(SDL_PollEvent(&event)) {
			switch(event.type)
			{
				case SDL_QUIT:
					continuerBoucle = 0;
					break;
				
				case SDL_KEYDOWN:	/* Evenement du clavier */
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE: /* Appui sur la touche Echap */
							continuerBoucle = 0;
							break;
					
						case SDLK_s:	/* Appui sur la touche s */
							edit_e = editeNiveau(FICHIER_NIVEAUX, FICHIER_TEMP, &level, numLvl, hauteurNiveau);
							if(edit_e == EditionNoError) {
								printf("Niveau %d sauvegarde.\n", numLvl);
							}
							else {
								fprintf(stderr, "(%d) Erreur de sauvegarde du niveau %d.\n", edit_e, numLvl);
							}
							break;

						default: break;
					}
					break;

				case SDL_MOUSEBUTTONDOWN: /* Evenement de la souris (appui d'un bouton) */
					switch(event.button.button)
					{
						case SDL_BUTTON_LEFT:		/* Appui sur le bouton gauche de la souris */
							lEnfoncee = 1;
							break;
						case SDL_BUTTON_RIGHT:		/* Appui sur le bouton droit de la souris */
							rEnfoncee = 1;
							break;
						case SDL_BUTTON_WHEELUP:	/* Impulsion molette vers le haut */
							molette++;
							break;
						case SDL_BUTTON_WHEELDOWN:	/* Impulsion molette vers le bas */
							molette--;
							break;
						default: break;
					}
					break;

				case SDL_MOUSEBUTTONUP:	/* Evenement de la souris (relachement d'un bouton) */
					switch(event.button.button)
					{
						case SDL_BUTTON_LEFT:		/* Relachement du bouton gauche de la souris */
							lEnfoncee = 0;
							break;
						case SDL_BUTTON_RIGHT:		/* Relachement du bouton droit de la souris */
							rEnfoncee = 0;
							break;
						default: break;
					}
					break;
				default: break;
			}
		}

		objetSelect = ObjetSelect(molette);
		if(lEnfoncee) {	/* Bouton gauche de la souris enfonce */
			afficheNiveau(&level, largeurNiveau, hauteurNiveau);
			dessinerElement(objetSelect, event.button.x, event.button.y, ecran, &tableauSprites, &level, largeurNiveau, hauteurNiveau); /* dessiner objetSelect */
		}
		if(rEnfoncee) {	/* Bouton droit de la souris enfonce */
			afficheNiveau(&level, largeurNiveau, hauteurNiveau);
			dessinerElement(S_SOL, event.button.x, event.button.y, ecran, &tableauSprites, &level, largeurNiveau, hauteurNiveau);	/* dessiner sol */
		}

		dessinerDescriptif(ecran, objetSelect, &tableauSprites);	/* Actualisation de la partie descriptive de l'en tete */
		SDL_Flip(ecran);	
		
		SDL_Delay(20);
	}

	freeNiveau(&level, hauteurNiveau);
	freeSDL(&tableauSprites);
	return EXIT_SUCCESS;
}
				
