#include "level.h"

/**
 * \fn LevelError getTailleNiveau(FILE* niveaux, int numero, int* largeur, int* hauteur)
 * \brief Donne la largeur et la hauteur du niveau desire
 *
 * \param niveaux Pointeur sur le fichier de niveaux
 * \param numero Numero du niveau dont on doit determiner les dimensions
 * \param largeur Pointeur sur l'entier qui contiendra la largeur du niveau
 * \param hauteur Pointeur sur l'entier qui contiendra la hauteur du niveau
 * \return Renvoie un code d'erreur, NoError si tout s'est bien passe
 */
LevelError getTailleNiveau(FILE* niveaux, int numero, int* largeur, int* hauteur, fpos_t* positionNiveau) {
	int level_courant = 0;
	char chaine[NB_BLOCS_LARGEUR+1]= {0};
	char copierAutorise = 0;
	int i = 0;

	*largeur = 0;
	*hauteur = 0;

	while (fgets(chaine, NB_BLOCS_LARGEUR+1, niveaux) != NULL) { 
		/* si on ne trouve pas ";LEVEL x" sur la ligne, on passe a la ligne suivante */
		if (sscanf(chaine, ";LEVEL %d", &level_courant) <= 0 && copierAutorise == 0) {
			continue;
		}

		if (level_courant == numero && copierAutorise == 0) {
			fgetpos(niveaux, positionNiveau);
			copierAutorise = 1;
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
		if (chaine[0] == ';') {
			fgetpos(niveaux, positionNiveau);
			continue;
		}

		i = 0;
		while(chaine[i] != '\n' && chaine[i] != '\0') {
			i++;
		}
		i++;

		*largeur = (i > *largeur) ? i : *largeur;
		
		(*hauteur)++;
	}
	(*hauteur)++;

	return NoError;
}

LevelError alloueNiveau(Niveau* niveau, int largeur, int hauteur) {
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

	return NoError;
}

LevelError freeNiveau(Niveau* n) {
	return NoError;
}

LevelError remplirNiveau(FILE* levels, fpos_t* position, Niveau* n, int w, int h) {
	LevelError code_e = NoError;
	int j = 0;
	char* buff = (char*)calloc(w, sizeof(char));
	
	fsetpos(levels, position);
	while (j<h && fgets(buff, w+1, levels) != NULL) { 	/* +1 pour '\n' */
		code_e = caractereValide(n, buff, j, w);
		if(code_e != NoError) {
			fprintf(stderr, "Caractere invalide present.\n");
			return CaractereInconnu;
		}
		j++;
	}

	return NoError;
}

LevelError caractereValide(Niveau* niveau, char* ligne, int numeroLigne, int largeur) {
	int k = 0;

	/* nous verifions que chaque caractere de la ligne est autorise */
	for(k = 0; k < largeur; k++) {
		switch (ligne[k]) {
			case JOUEUR:		/* joueur */
			case MUR:		/* mur */
			case CIBLE:		/* cible */
			case CAISSE:		/* caisse */
			case SOL:		/* sol */
			case CAISSE_CIBLE:	/* caisse sur cible */
			case JOUEUR_CIBLE:	/* joueur sur cible */
				(*niveau)[numeroLigne][k] = ligne[k];
				break;
			default:	/* caractere autre */
				break;
		}
		if(ligne[k] == '\n') {
			break;
		}
	}

	return NoError;
}

LevelError readLevel(char* path, Niveau* niveau, int numero, int* largeurNiveau, int* hauteurNiveau) {
	LevelError code_e = NoError;
	fpos_t positionDansFichier;

	*hauteurNiveau = 0;
	*largeurNiveau = 0;
	
	/* Ouverture du fichier */
	FILE* levels = fopen("levels.lvl", "r");
	if(levels == NULL) {
		fprintf(stderr, "Fichier de niveaux introuvable : %s.\n", path);
		return FichierIntrouvable;
	}
	
	/* Recuperation de la taille du niveau */
	code_e = getTailleNiveau(levels, numero, largeurNiveau, hauteurNiveau, &positionDansFichier);
	if(code_e != NoError) {
		fprintf(stderr, "Erreur lors de la lecture des dimensions du niveau %d.\n", numero);
		return ErreurDimensions;
	}

	code_e = alloueNiveau(niveau, *largeurNiveau, *hauteurNiveau);
	if(code_e != NoError) {
		fprintf(stderr, "Erreur lors de l'allocation memoire.\n");
		return AllocationMemoire;
	}

	code_e = remplirNiveau(levels, &positionDansFichier, niveau, *largeurNiveau, *hauteurNiveau);
	if(code_e != NoError) {
		fprintf(stderr, "Caractere inconnu present dans le niveau %d.\n", numero);
		return CaractereInconnu;
	}

	/* Fermeture du fichier */
	fclose(levels);
	return NoError;
}
