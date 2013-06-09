/** 
 * \mainpage Pokeban Documentation
 * 
 * \section intro_sec Introduction
 * 
 * Ceci est la documentation du jeu Pokeban.
 *
 */

/**
 * \file main.c
 * \brief Programme principal, le jeu Pokeban.
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "sokoban.h"
#include "level.h"
#include "mouvement.h"
#include "graphic.h"
#include "save.h"

void afficheNiveau(Niveau* n, int w, int h) {
	int i = 0, j = 0;	
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			printf("%c", (*n)[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]) {
	unsigned int largeurNiveau = 0, hauteurNiveau = 0;
	unsigned int numLvl = 1;
	Niveau level;
	LevelError code_e;
	CoupsJoues pileDeCoups = NULL;
	Coup* coupDepile = NULL;
	PileCible pileDeCibles = NULL;

	char cheminFichierSolution[27] = {0};
	FILE* fichierSolution = NULL;

	Position positionJoueur;
	unsigned int nbPoussee = 0, nbMvt = 0;

	char continuerBoucle = 1;

	SDLInitError sdl_e;
	SDL_Surface *ecran;
	SDL_Event event;
	SDL_Surface** tableauSprites = NULL;
	
	if(argc > 1) {
		numLvl = atoi(argv[1]);
	}
	
	code_e = readLevel(FICHIER_NIVEAUX, &level, numLvl, &largeurNiveau, &hauteurNiveau, &positionJoueur, &pileDeCibles);
	if(code_e != NoError) {
		fprintf(stderr, "(%d) Erreur de lecture du fichier de niveau \"%s\".\n", code_e, FICHIER_NIVEAUX);
		libererPileCible(&pileDeCibles);
		freeNiveau(&level, hauteurNiveau);
		exit(EXIT_FAILURE);
	}

	sdl_e = initSDL(&ecran);
	if(sdl_e != InitOk) {
		fprintf(stderr, "(%d) Erreur lors de initSDL(SDL_Surface**).\n", sdl_e);
		libererPileCible(&pileDeCibles);
		freeNiveau(&level, hauteurNiveau);
		freeSDL(NULL);
		exit(EXIT_FAILURE);
	}

	tableauSprites = (SDL_Surface**)malloc(NB_SPRITE * sizeof(SDL_Surface*));
	if(tableauSprites == NULL) {
		fprintf(stderr, "Erreur d'allocation du tableau de sprites.\n");
		libererPileCible(&pileDeCibles);
		freeNiveau(&level, hauteurNiveau);
		freeSDL(NULL);
		exit(EXIT_FAILURE);
	}

	sdl_e = loadSprites(&tableauSprites);
	if(sdl_e != InitOk) {
		fprintf(stderr, "(%d) Erreur de chargement des sprites dans le tableau de sprites.\n", sdl_e);
		libererPileCible(&pileDeCibles);
		freeNiveau(&level, hauteurNiveau);
		freeSDL(&tableauSprites);
		exit(EXIT_FAILURE);
	}

	printf("Niveau %d\n", numLvl);
	afficheNiveau(&level, largeurNiveau, hauteurNiveau);

	dessineEntete(ecran);
	dessineCompteurs(ecran, nbMvt, nbPoussee);
	dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);

	while (continuerBoucle)
	{
		while(SDL_PollEvent(&event)) {
			switch(event.type)
			{
				case SDL_QUIT:
					continuerBoucle = 0;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							continuerBoucle = 0;
							break;
						case SDLK_UP:
							arrowKeyDownHandler(HAUT, &level, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_DOWN:
							arrowKeyDownHandler(BAS, &level, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_RIGHT:
							arrowKeyDownHandler(DROITE, &level, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_LEFT:
							arrowKeyDownHandler(GAUCHE, &level, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_r:
							libererPile(&pileDeCoups);
							code_e = updateLevel(LevelReset, FICHIER_NIVEAUX, &level, &positionJoueur, &pileDeCibles, &numLvl, &largeurNiveau, &hauteurNiveau);
							if(code_e != NoError) {
								fprintf(stderr, "(%d) Erreur lors du redemarrage du niveau.\n", code_e);
								libererPileCible(&pileDeCibles);
								freeNiveau(&level, hauteurNiveau);
								freeSDL(&tableauSprites);
								exit(EXIT_FAILURE);
							}

							nbMvt = 0;
							nbPoussee = 0;
							break;
						case SDLK_e:
							libererPile(&pileDeCoups);
							code_e = updateLevel(LevelPrevious, FICHIER_NIVEAUX, &level, &positionJoueur, &pileDeCibles, &numLvl, &largeurNiveau, &hauteurNiveau);
							if(code_e != NoError) {
								fprintf(stderr, "(%d) Erreur lors du passage au niveau precedent.\n", code_e);
								libererPileCible(&pileDeCibles);
								freeNiveau(&level, hauteurNiveau);
								freeSDL(&tableauSprites);
								exit(EXIT_FAILURE);
							}

							nbMvt = 0;
							nbPoussee = 0;
							printf("Niveau %d\n", numLvl);
							afficheNiveau(&level, largeurNiveau, hauteurNiveau);
							break;
						case SDLK_t:
							sprintf(cheminFichierSolution, "./save/level%d.sol", numLvl);
							fichierSolution = fopen(cheminFichierSolution, "r");
							if(fichierSolution != NULL) {
								fclose(fichierSolution);

								libererPile(&pileDeCoups);
								code_e = updateLevel(LevelNext, FICHIER_NIVEAUX, &level, &positionJoueur, &pileDeCibles, &numLvl, &largeurNiveau, &hauteurNiveau);
								if(code_e != NoError) {
									fprintf(stderr, "(%d) Erreur lors du passage au niveau suivant.\n", code_e);
									libererPileCible(&pileDeCibles);
									freeNiveau(&level, hauteurNiveau);
									freeSDL(&tableauSprites);
									exit(EXIT_FAILURE);
								}

								nbMvt = 0;
								nbPoussee = 0;
								printf("Niveau %d\n", numLvl);
								afficheNiveau(&level, largeurNiveau, hauteurNiveau);
							}
							break;
						case SDLK_w: /* NB: ceci est la version windows, allez savoir pourquoi SDL prend le clavier en qwerty */
							if ((event.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL)
							{
								coupDepile = depilerCoup(&pileDeCoups);
								if(coupDepile != NULL) {
									deplacerObjetRetour(&level, *coupDepile, &nbMvt, &nbPoussee);
									calculPosition(&positionJoueur, (*coupDepile).direction, Negatif);
									free(coupDepile);
								}								
							}
							break;
						case SDLK_q: /* NB: ceci est la version windows, allez savoir pourquoi SDL prend le clavier en qwerty */
							creerFichierSauvegarde(Sauvegarde, numLvl, &pileDeCoups);
							break;
						case SDLK_a: /* NB: ceci est la version windows, allez savoir pourquoi SDL prend le clavier en qwerty */
							chargerFichierSauvegarde(numLvl, &pileDeCoups, &level, &positionJoueur, &pileDeCibles, &nbMvt, &nbPoussee);
							break;
						default: break;
					}
					
					if(toutesCiblesValidees(&pileDeCibles, &level) == CibleValidee) {
						printf("Niveau %d termine en %d mouvements et en %d poussees ! \n", numLvl, nbMvt, nbPoussee);
						creerFichierSauvegarde(Solution, numLvl, &pileDeCoups);
						libererPile(&pileDeCoups);
						code_e = updateLevel(LevelNext, FICHIER_NIVEAUX, &level, &positionJoueur, &pileDeCibles, &numLvl, &largeurNiveau, &hauteurNiveau);
						if(code_e != NoError) {
							fprintf(stderr, "(%d) Erreur lors du passage au niveau suivant.\n", code_e);
							libererPileCible(&pileDeCibles);
							freeNiveau(&level, hauteurNiveau);
							freeSDL(&tableauSprites);
							exit(EXIT_FAILURE);
						}

						nbMvt = 0;
						nbPoussee = 0;
						printf("Niveau %d\n", numLvl);
						afficheNiveau(&level, largeurNiveau, hauteurNiveau);
					}
					break;
				default: break;
			}
		}

		effacerNiveau(ecran);
		dessineEntete(ecran);
		dessineCompteurs(ecran, nbMvt, nbPoussee);
		dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);

		SDL_Flip(ecran);
		SDL_Delay(20);
	}

	libererPile(&pileDeCoups);
	libererPileCible(&pileDeCibles);
	freeNiveau(&level, hauteurNiveau);
	freeSDL(&tableauSprites);
	return EXIT_SUCCESS;
}
