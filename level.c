#include "level.h"

LevelError getTailleNiveau(FILE* niveaux, int numero, int* largeur, int* hauteur) {

	return 0;
}

LevelError alloueNiveau(Niveau* niveau, int largeur, int hauteur) {
	LevelError code_e = NoError;
	char* ligne = NULL;
	int i = 0;

	/* Initialisation du tableau 2D de caracteres */
	*niveau = (Niveau)calloc(hauteur, sizeof(char*));
	if(*niveau == NULL) {
		fprintf(stderr, "Probleme d'allocation memoire du niveau.\n");
		return AllocationMemoire;
	}
	
	/* Initialisation de chaque ligne */
	for(i = 0; i < hauteur; i++) {
		ligne = (char*)calloc(largeur, sizeof(char));
		if(ligne == NULL) {
			fprintf(stderr, "Probleme d'allocation memoire d'une ligne du niveau.\n");
			freeNiveau(niveau);
			return AllocationMemoire;
		}
		(*niveau)[i] = ligne;
	}
}

LevelError freeNiveau(Niveau* n) {

}

LevelError remplirNiveau(Niveau* n) {
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

		if (level_courant == numero && copierAutorise == 0) {
			/* ";LEVEL x" existe, si x est le numero du level voulu, on autorise la copie pour les prochaines lignes */
			copierAutorise = 1;
			j++;
			continue;
		}
		else if(level_courant != numero && copierAutorise == 1) {
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

		j++;
	}
}

LevelError caractereValide(Niveau* niveau, char* ligne, int numeroLigne, int largeur) {
	int k = 0;

	/* nous verifions que chaque caractere de la ligne est autorise */
	for(k = 0; k < largeur; k++) {
		switch (ligne[k]) {
			case '\n':		/* nous sommes a la fin de la ligne, passons a la ligne suivante */
				break;
			case Joueur:		/* joueur */
			case Mur:		/* mur */
			case Cible:		/* cible */
			case Caisse:		/* caisse */
			case Sol:		/* sol */
			case CaisseCible:	/* caisse sur cible */
			case JoueurCible:	/* joueur sur cible */
				(*niveau)[numeroLigne][k] = ligne[k];
				break;
			default:	/* caractere autre */
				/* on liberera ici les lignes deja completees ainsi que le niveau de la memoire */
				fprintf(stderr, "Caractere inconnu.\n");
				return CaractereInconnu;
		}

		if(ligne[k] == '\n') {
			break;
		}
	}
}

LevelError readLevel(char* path, NIVEAU* niveau, int numero) {
	int hauteurNiveau = 0, largeurNiveau = 0;
	LevelError code_e = NoError;
	
	/* Ouverture du fichier */
	FILE* levels = fopen("levels.lvl", "r");
	if(levels == NULL) {
		fprintf(stderr, "Fichier de niveaux introuvable : %s.\n", path);
		return FichierIntrouvable;
	}
	
	/* Recuperation de la taille du niveau */
	code_e = getTailleNiveau(levels, &w, &h);
	if(code_e != NoError) {
		fprintf(stderr, "Erreur lors de la lecture des dimensions du niveau %d.\n", numero);
		return ErreurDimensions;
	}

	code_e = alloueNiveau(niveau, w, h);
	
	/* Fermeture du fichier */
	fclose(levels);
	return NoError;
}
