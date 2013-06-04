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
	Niveau level;
	int largeurNiveau = 0, hauteurNiveau = 0;
	
	int poussee = 0, mvt = 0;
	char result = 0;
	
	char numlvl = 1;
	char continuer = 1;
	
	SDL_Surface *ecran;
	SDL_Event event;
	Position positionJoueur;
	SDL_Surface** tableauSprites = (SDL_Surface**)malloc(NB_SPRITE * sizeof(SDL_Surface*));
	if(tableauSprites == NULL) {
		fprintf(stderr, "Erreur d'allocation du tableau de sprites.\n");
		exit(EXIT_FAILURE);
	}
	
	if(argc > 1) {
		numlvl = atoi(argv[1]);
	}

	#ifdef SOUND
	FMOD_SYSTEM* syst = NULL;
	FMOD_SOUND* sound = NULL;
	FMOD_System_Create(&syst);
	FMOD_System_Init(syst, 1, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateSound(syst, "music.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sound);
	FMOD_System_PlaySound(syst, FMOD_CHANNEL_FREE, sound, 0, NULL);
	#endif
	
	CoupsJoues pileDeCoups = (CoupsJoues)NULL;
	
	LevelError code_e = readLevel("levels.lvl", &level, numlvl, &largeurNiveau, &hauteurNiveau, &positionJoueur);
	if(code_e != NoError) {
		exit(EXIT_FAILURE);
	}
	printf("PositionJoueur: %d %d\n", positionJoueur.x, positionJoueur.y);	

	initSDL(&ecran);
	loadSprites(&tableauSprites);
	dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);

	while (continuer)
	{
		while(SDL_PollEvent(&event)) {
			continuer = !clavierHandler(&event, &level, &positionJoueur, &pileDeCoups, &mvt, &poussee);
			effacerNiveau(ecran);
			dessineNiveau(&level, ecran, largeurNiveau, hauteurNiveau, &tableauSprites);
			SDL_Flip(ecran);
		}
	}

	#ifdef SOUND
	FMOD_Sound_Release(sound);
	FMOD_System_Close(syst);
	FMOD_System_Release(syst);
	#endif
	
	freeSDL(&tableauSprites);
	return EXIT_SUCCESS;
}
