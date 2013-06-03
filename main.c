#include <stdlib.h>
#include <stdio.h>

#include "level.h"

int main(int argc, char* argv[]) {
	Niveau level;
	int largeurNiveau = 0, hauteurNiveau = 0;

	LevelError code_e = readLevel("levels.lvl", &level, 1, &largeurNiveau, &hauteurNiveau);
	if(code_e != NoError) {
		exit(EXIT_FAILURE);
	}

	int i = 0, j = 0;	
	for(i = 0; i<hauteurNiveau; i++) {
		for(j = 0; j<largeurNiveau; j++) {
			printf("%c", level[i][j]);
		}
		printf("\n");
	}
	printf("%d\n", largeurNiveau);

	return EXIT_SUCCESS;
}
