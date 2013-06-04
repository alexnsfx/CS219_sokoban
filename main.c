#include <stdlib.h>
#include <stdio.h>

#ifdef SOUND
#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "sokoban.h"
#include "level.h"
#include "mouvement.h"
#include "graphic.h"

#define COLOR_BIT 32

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
	
	SDL_Surface* ecran, cellule;
	SDL_Event event;
	SDL_Rect pos;
	
	if(argc > 1) {
		numlvl = atoi(argv[1]);
	}
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Erreur d'initialisation de la SDL ! %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_WM_SetCaption("Sokoban ! =D", NULL);
	ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, COLOR_BIT, SDL_HWSURFACE);
	if(ecran == NULL) {
		fprintf(stderr, "Impossible de charger le mode video ! %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

	cellule = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, COLOR_BIT, 0, 0, 0, 0);
	Uint32 couleurObjets[] = { 	SDL_MapRGB(cellule->format, 0, 0, 0), 
								SDL_MapRGB(cellule->format, 64, 0, 0),
								SDL_MapRGB(cellule->format, 255, 0, 0),
								SDL_MapRGB(cellule->format, 255, 128, 0),
								SDL_MapRGB(cellule->format, 255, 255, 255),
								SDL_MapRGB(cellule->format, 0, 255, 0),
								SDL_MapRGB(cellule->format, 128, 128, 128) };

	#ifdef SOUND
	FMOD_SYSTEM* syst = NULL;
	FMOD_SOUND* sound = NULL;
	FMOD_System_Create(&syst);
	FMOD_System_Init(syst, 1, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateSound(syst, "", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sound);
	FMOD_System_PlaySound(syst, FMOD_CHANNEL_FREE, sound, 0, NULL);
	#endif
	
	CoupsJoues pileDeCoups = (CoupsJoues)NULL;
	
	LevelError code_e = readLevel("levels.lvl", &level, numlvl, &largeurNiveau, &hauteurNiveau);
	if(code_e != NoError) {
		exit(EXIT_FAILURE);
	}

	afficheNiveau(&level, largeurNiveau, hauteurNiveau);
	
	int i, j;
	for(j = 0; j < hauteurNiveau; j++) {
		for(i = 0; i < largeurNiveau; i++) {
			pos.x = i * TAILLE_BLOC;
			pos.y = j * TAILLE_BLOC;
			SDL_FillRect(cellule, NULL, couleurObjets[0]);
			SDL_BlitSurface(cellule, NULL, ecran, &pos);
		}
	}
	
	SDL_Flip(ecran);

	
	result = deplacementPossible(&level, 6, 15, 0, BAS);
	printf("%d\n", result);
	
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						break;
					case SDLK_DOWN:
						break;
					case SDLK_RIGHT:
						break;
					case SDLK_LEFT:
						break;
				}
				break;
		}
	}

	#ifdef SOUND
	FMOD_Sound_Release(sound);
	FMOD_System_Close(syst);
	FMOD_System_Release(syst);
	#endif
	
	SDL_FreeSurface(cellule);
	SDL_Quit();
	return EXIT_SUCCESS;
}
