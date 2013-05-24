#include "level.h"

char getTailleNiveau(FILE* niveaux, int* largeur, int* hauteur) {

	return 0;
}

char** readLevel(char* path, int numero) {
	int h = 0, w = 0, code_e = 0;
	
	/* Ouverture du fichier */
	FILE* levels = fopen("levels.lvl");
	if(levels == NULL) {
		fprintf(stderr, "Fichier de niveaux introuvable : %s.\n", path);
		return NULL;
	}
	
	/* Recuperation de la taille du niveau */
	code_e = getTailleNiveau(levels, &h, &w);
	if(code_e != 0) {
		fprintf(stderr, "Erreur lors de la lecture des dimensions du niveau %d.\n", numero);
		return NULL;
	}
	
	/* Initialisation du tableau 2D de caracteres */
	char** niveau = (char**)calloc(h, sizeof(char*));
	if(niveau == NULL) {
		fprintf(stderr, "Probleme d'allocation memoire du niveau.\n");
		return NULL;
	}
	
	/* Initialisation de chaque ligne */
	char* ligne = NULL;
	int i = 0;
	for(i=0; i<h; i++) {
		ligne = (char*)calloc(w, sizeof(char));
		if(ligne == NULL) {
			fprintf(stderr, "Probleme d'allocation memoire d'une ligne du niveau.\n");
			free(niveau);
			return NULL;
		}
		niveau[i] = ligne;
	}
	
	/* Remplissage du tableau */
	int j = 0, level_courant = 0, k = 0;
	char copierAutorise = 0;
	char* buff = (char*)calloc(w, sizeof(char));
	
	while (j<h && fgets(buff, w, levels) != NULL) { 
		/* si on ne trouve pas ";LEVEL x" sur la ligne, on passe a la ligne suivante */
		if (sscanf(buff, ";LEVEL %d", &level_courant) <= 0 && copierAutorise == 0) {
			j++;
			continue;
		}

		if (level_courant == numero) {
			/* ";LEVEL x" existe, si x est le numero du level voulu, on autorise la copie pour les prochaines lignes */
			copierAutorise = 1;
			j++;
			continue;
		}
		else {
			/* ";LEVEL x" existe, mais x n'est pas le numero du level voulu */
			/* on a donc fini de lire le niveau, on sort du while */
			copierAutorise = 0;
			break;
		}
		
		/* ";LEVEL x" n'existe pas sur cette ligne, mais la copie est autorisee */
		/* si le premier caractere est un ';', nous sommes dans un champ AUTHOR ou COMMENT, il faut passer a la ligne suivante */
		if (buff[0] == ';') {
			j++;
			continue;
		}
		
		/* nous sommes a la premiere ligne du niveau */
		/* nous verifions que chaque caractere de la ligne est autorise */
		for(k = 0; k < w; k++) {
			switch (buff[k]) {
				case '@':	/* joueur */
				case '#':	/* mur */
				case '.':	/* cible */
				case '$':	/* caisse */
				case ' ':	/* sol */
				case '+':	/* caisse sur cible */
				case '=':	/* joueur sur cible */
					niveau[j][k] = buff[k];
					break;
				default:	/* caractere autre */
					/* on liberera ici les lignes deja completees ainsi que le niveau de la memoire */
					fprintf(stderr, "Caractere inconnu.\n");
					return NULL;
			}
		}
		j++;
	}
	
	/* Fermeture du fichier */
	fclose(levels);
	return niveau;
}
