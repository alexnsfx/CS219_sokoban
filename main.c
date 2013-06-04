#include <stdlib.h>
#include <stdio.h>

#ifdef SOUND
#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>
#endif

#include <SDL/SDL.h>

#include "sokoban.h"
#include "level.h"
#include "mouvement.h"
#include "graphic.h"

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
	int largeurNiveau = 0, hauteurNiveau = 0;
	char numLvl = 1;
	Niveau level;
	CoupsJoues pileDeCoups = (CoupsJoues)NULL;

	int nbPoussee = 0, nbMvt = 0;
	char continuerBoucle = 1;
	
	Position positionJoueur;
	SDL_Surface *ecran;
	SDL_Event event;
	SDL_Surface** tableauSprites = (SDL_Surface**)malloc(NB_SPRITE * sizeof(SDL_Surface*));
	if(tableauSprites == NULL) {
		fprintf(stderr, "Erreur d'allocation du tableau de sprites.\n");
		exit(EXIT_FAILURE);
	}
	
	if(argc > 1) {
		numLvl = atoi(argv[1]);
	}

	#ifdef SOUND
	FMOD_SYSTEM* syst = NULL;
	FMOD_SOUND* sound = NULL;
	FMOD_System_Create(&syst);
	FMOD_System_Init(syst, 1, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateSound(syst, "music.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sound);
	FMOD_System_PlaySound(syst, FMOD_CHANNEL_FREE, sound, 0, NULL);
	#endif
	
	
	LevelError code_e = readLevel("levels.lvl", &level, numLvl, &largeurNiveau, &hauteurNiveau, &positionJoueur);
	if(code_e != NoError) {
		exit(EXIT_FAILURE);
	}
	afficheNiveau(&level, largeurNiveau, hauteurNiveau);
	printf("PositionJoueur: %d %d\n", positionJoueur.x, positionJoueur.y);	
	
	initSDL(&ecran);
	loadSprites(&tableauSprites);
	dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);

	while (continuerBoucle)
	{
		while(SDL_PollEvent(&event)) {
			switch((*event).type)
			{
				case SDL_QUIT:
					continuerBoucle = 0;
				case SDL_KEYDOWN:
					switch ((*event).key.keysym.sym)
					{	
						case SDLK_ESCAPE:
							continuerBoucle = 0;
						case SDLK_UP:
							keyDownHandler(HAUT, n, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_DOWN:
							keyDownHandler(BAS, n, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_RIGHT:
							keyDownHandler(DROITE, n, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						case SDLK_LEFT:
							keyDownHandler(GAUCHE, n, &positionJoueur, &pileDeCoups, &nbMvt, &nbPoussee);
							break;
						default: break;
					}
					break;
				default: break;
			}
			
			effacerNiveau(ecran);
			dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);
			SDL_Flip(ecran);
		}
		SDL_Delay(20);
	}

	#ifdef SOUND
	FMOD_Sound_Release(sound);
	FMOD_System_Close(syst);
	FMOD_System_Release(syst);
	#endif
	
	freeSDL(&tableauSprites);
	return EXIT_SUCCESS;
}
