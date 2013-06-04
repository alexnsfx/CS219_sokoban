#include <stdlib.h>
#include <stdio.h>

#ifdef SOUND
#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>
#endif

#include "level.h"
#include "mouvement.h"

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
	if(argc > 1) {
		numlvl = atoi(argv[1]);
	}
	
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
	
	result = deplacementPossible(&level, 6, 15, 0, BAS);
	printf("%d\n", result);
	
	#ifdef SOUND
	while(1);
	
	FMOD_Sound_Release(sound);
	FMOD_System_Close(syst);
	FMOD_System_Release(syst);
	#endif
	
	return EXIT_SUCCESS;
}
