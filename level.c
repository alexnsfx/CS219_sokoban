#include "level.h"

/**
 * \fn ResultatCible toutesCiblesValidees(PileCible* pile, Niveau* niveau)
 * \brief Indique si toutes les Cibles de la pile sont valides.
 *
 * \param pile Pointeur sur la pile de Cible a verifier.
 * \param niveau Pointeur sur le Niveau.
 *
 * \return Renvoie CibleValidee si toutes les cibles de la pile sont valides, CibleNonValidee sinon.
 */
ResultatCible toutesCiblesValidees(PileCible* pile, Niveau* niveau) {
	ResultatCible resCibleSuivante = CibleValidee;

	if(pile == NULL || niveau == NULL) {
		return CibleNonValidee;
	}

	if((*pile)->next != NULL) {
		resCibleSuivante = toutesCiblesValidees(&((*pile)->next), niveau);
	}

	if((*niveau)[(*pile)->y][(*pile)->x] == CAISSE_CIBLE) {
		(*pile)->validee = CibleValidee;
	}
	else {
		(*pile)->validee = CibleNonValidee;
	}

	return (ResultatCible)((*pile)->validee && resCibleSuivante);
}

/**
 * \fn PileError empilerCible(PileCible* pile, unsigned int x, unsigned int y, char validee)
 *
 * \brief Empile une Cible sur une pile.
 *
 * \param pile Pointeur sur la pile.
 * \param x Abscisse de la cible dans le Niveau.
 * \param y Ordonnee de la cible dans le Niveau.
 * \param validee 1 si une CAISSE est presente sur la Cible, 0 sinon.
 *
 * \return Renvoie AllocError si erreur d'allocation de la nouvelle cible, UndefinedPile si pile est nul, NoError si tout s'est bien passe.
 */
PileError empilerCible(PileCible* pile, unsigned int x, unsigned int y, char validee) {
	Cible* nouvelleCible = NULL;	
	
	if(pile == NULL) {
		return UndefinedPile;
	}
	
	nouvelleCible = (Cible*)malloc(sizeof(Cible));
	if(nouvelleCible == NULL) {
		return AllocError;
	}
	
	nouvelleCible->x = x;
	nouvelleCible->y = y;
	nouvelleCible->validee = validee;
	nouvelleCible->next = *pile;
	*pile = nouvelleCible;
	
	return Ok;
}

/**
 * \fn void libererPileCible(PileCible* pile)
 *
 * \brief Libere une pile de Cible de la memoire.
 *
 * \param pile Pointeur sur la pile a liberer.
 */
void libererPileCible(PileCible* pile) {
	if(pile == NULL || *pile == NULL) {
		return;
	}

	if((*pile)->next != NULL) {
		libererPileCible(&((*pile)->next));
		(*pile)->next = NULL;
	}
	free(*pile);
	*pile = NULL;
}

/**
 * \fn void getTailleNiveau(FILE* niveaux, unsigned int numero, unsigned int* largeur, unsigned int* hauteur, fpos_t* positionNiveau)
 * \brief Donne la largeur et la hauteur du niveau desire.
 *
 * \param niveaux Pointeur sur le fichier de niveaux.
 * \param numero Numero du niveau dont on doit determiner les dimensions.
 * \param largeur Pointeur sur l'entier qui contiendra la largeur du niveau.
 * \param hauteur Pointeur sur l'entier qui contiendra la hauteur du niveau.
 * \param positionNiveau Pointeur sur fpos_t, contenant la position du niveau dans le fichier.
 */
void getTailleNiveau(FILE* niveaux, unsigned int numero, unsigned int* largeur, unsigned int* hauteur, fpos_t* positionNiveau) {
	int level_courant = 0;
	char chaine[BUFFER_WIDTH]= {0};
	char copierAutorise = 0;
	int i = 0;

	if(niveaux == NULL || largeur == NULL || hauteur == NULL || positionNiveau == NULL) {
		return;
	}

	*largeur = 0;
	*hauteur = 0;

	while (fgets(chaine, BUFFER_WIDTH, niveaux) != NULL) {
	
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

		*largeur = (i > *largeur) ? i : *largeur;
		(*hauteur)++;
	}
}

/**
 * \fn LevelError alloueNiveau(Niveau* niveau, unsigned int largeur, unsigned int hauteur)
 *
 * \brief Alloue l'espace memoire necessaire pour stocker le Niveau.
 *
 * \param niveau Pointeur sur le Niveau a allouer.
 * \param largeur Pointeur sur l'entier qui contiendra la largeur du niveau.
 * \param hauteur Pointeur sur l'entier qui contiendra la hauteur du niveau.
 *
 * \return Renvoie UndefinedLevel si niveau est nul, AllocationMemoire si erreur d'allocation memoire, NoError sinon.
 */
LevelError alloueNiveau(Niveau* niveau, unsigned int largeur, unsigned int hauteur) {
	char* ligne = NULL;
	int i = 0;

	if(niveau == NULL) {
		return UndefinedLevel;
	}

	/* Initialisation du tableau 2D de caracteres */
	*niveau = (Niveau)calloc(hauteur, sizeof(char*));
	if(*niveau == NULL) {
		fprintf(stderr, "Probleme d'allocation memoire du niveau.\n");
		return AllocationMemoire;
	}
	
	/* Initialisation de chaque ligne */
	for(i = 0; i < hauteur; i++) {
		ligne = (char*)malloc(largeur * sizeof(char));
		if(ligne == NULL) {
			fprintf(stderr, "Probleme d'allocation memoire de la ligne %d du niveau.\n", i+1);
			freeNiveau(niveau, largeur);
			return AllocationMemoire;
		}
		memset(ligne, SOL, largeur * sizeof(char));
		(*niveau)[i] = ligne;
	}

	return NoError;
}

/**
 * \fn void freeNiveau(Niveau* n, unsigned int hauteur)
 *
 * \brief Libere le Niveau de la memoire.
 *
 * \param n Pointeur sur le Niveau a allouer.
 * \param hauteur Nombre de lignes du Niveau.
 */
void freeNiveau(Niveau* n, unsigned int hauteur) {
	int i;
	if(n == NULL || *n == NULL) {
		return;
	}
	for(i = 0; i < hauteur; i++) {
		if((*n)[i] != NULL) {
			free((*n)[i]);
		}
	}
	free(*n);
}

/**
 * \fn LevelError remplirNiveau(FILE* levels, fpos_t* position, Niveau* n, unsigned int w, unsigned int h, Position* posJoueur, PileCible* pileCible)
 *
 * \brief Remplit le Niveau avec les caracteres du fichier.
 *
 * \param levels Pointeur sur le fichier definissant les niveaux.
 * \param position Pointeur sur la position du niveau dans le fichier de niveaux.
 * \param n Pointeur sur le Niveau a remplir.
 * \param w Nombre de colonnes du Niveau.
 * \param h Nombre de lignes du Niveau.
 * \param posJoueur Pointeur sur la Position du joueur, assignee des que le joueur est trouve dans le fichier.
 * \param pileCible Pointeur sur une pile de Cible, chaque Cible trouvee dans le niveau y est empilee.
 *
 * \return Renvoie UndefinedLevel si n est nul, UndefinedParameter si un des autres parametres est nul, le code d'erreur de caractereValide si son execution echoue, NoError sinon.
 */
LevelError remplirNiveau(FILE* levels, fpos_t* position, Niveau* n, unsigned int w, unsigned int h, Position* posJoueur, PileCible* pileCible) {
	LevelError code_e = NoError;
	int j = 0;
	char* buff;
	
	if(levels == NULL || position == NULL || posJoueur == NULL || pileCible == NULL) {
		return UndefinedParameter;
	}
	if(n == NULL) {
		return UndefinedLevel;
	}

	buff = (char*)calloc(w+2, sizeof(char));		/* +2 pour '\n' et '\0' envoyes par fgets */
	
	fsetpos(levels, position);
	while (j<h && fgets(buff, w+2, levels) != NULL) { 	/* +2 pour '\n' et '\0' envoyes par fgets */
		code_e = caractereValide(n, buff, j, w, posJoueur, pileCible);
		if(code_e != NoError) {
			fprintf(stderr, "(%d) Erreur a la validation de la ligne %d du niveau.\n", code_e, j+1);
			return code_e;
		}
		j++;
	}

	return NoError;
}

/**
 * \fn LevelError caractereValide(Niveau* niveau, char* ligne, unsigned int numeroLigne, unsigned int largeur, Position* posJoueur, PileCible* pileCible)
 *
 * \brief Verifie que les caracteres de ligne sont valides. Assigne la Position du joueur quand il est trouve. Empile les Cible trouvees dans pileCible.
 *
 * \param niveau Pointeur sur le Niveau a remplir.
 * \param ligne Ligne a valider
 * \param numeroLigne Pointeur sur le Niveau a remplir.
 * \param largeur Nombre de colonnes du Niveau.
 * \param posJoueur Pointeur sur la Position du joueur, assignee des que le joueur est trouve dans le fichier.
 * \param pileCible Pointeur sur une pile de Cible, chaque Cible trouvee dans le niveau y est empilee.
 *
 * \return Renvoie UndefinedLevel si n est nul, UndefinedParameter si un des autres parametres est nul, AllocationMemoire si erreur d'empilage d'une Cible, CaractereInconnu si un caractere inconnu est present, NoError sinon.
 */
LevelError caractereValide(Niveau* niveau, char* ligne, unsigned int numeroLigne, unsigned int largeur, Position* posJoueur, PileCible* pileCible) {
	PileError pile_e = Ok;
	int k = 0;

	if(ligne == NULL || posJoueur == NULL || pileCible == NULL) {
		return UndefinedParameter;
	}
	if(niveau == NULL) {
		return UndefinedLevel;
	}

	/* nous verifions que chaque caractere de la ligne est autorise */
	for(k = 0; k < largeur; k++) {
		switch (ligne[k]) {
			case CIBLE:		/* cible */
				pile_e = empilerCible(pileCible, k, numeroLigne, CibleNonValidee);
				if(pile_e != Ok) {
					fprintf(stderr, "(%d) Erreur lors de l'empilage d'une cible.\n", pile_e);
					libererPileCible(pileCible);
					freeNiveau(niveau, largeur);
					return AllocationMemoire;
				}

				(*niveau)[numeroLigne][k] = ligne[k];	/* on copie le caractere dans le niveau */
				break;

			case CAISSE_CIBLE:	/* caisse sur cible */
				pile_e = empilerCible(pileCible, k, numeroLigne, CibleValidee);
				if(pile_e != Ok) {
					fprintf(stderr, "(%d) Erreur lors de l'empilage d'une cible.\n", pile_e);
					libererPileCible(pileCible);
					freeNiveau(niveau, largeur);
					return AllocationMemoire;
				}

				(*niveau)[numeroLigne][k] = ligne[k];	/* on copie le caractere dans le niveau */
				break;
			
			case JOUEUR:		/* joueur */
			case JOUEUR_CIBLE:	/* joueur sur cible */
				(*posJoueur).x = k;
				(*posJoueur).y = numeroLigne;
			case MUR:		/* mur */
			case CAISSE:		/* caisse */
			case SOL:		/* sol */
				(*niveau)[numeroLigne][k] = ligne[k];	/* on copie le caractere dans le niveau */
				break;
			case '\n':	/* fin d'une ligne de largeur inferieure a largeur */
				break;
			default:	/* caractere autre */
				fprintf(stderr, "Le caractere '%c' est inconnu.\n", ligne[k]);
				libererPileCible(pileCible);
				freeNiveau(niveau, largeur);
				return CaractereInconnu;
		}

		if(ligne[k] == '\n') {
			/* fin d'une ligne de largeur inferieure a largeur, on passe a la ligne suivante */
			break;
		}
	}

	return NoError;
}

/**
 * \fn LevelError readLevel(char* path, Niveau* niveau, unsigned int numero, unsigned int* largeurNiveau, unsigned int* hauteurNiveau, Position* posJoueur, PileCible* pileCible)
 *
 * \brief Ouvre le fichier situe a path. Alloue l'espace memoire necessaire pour stocker le Niveau designe par numero. Remplit le Niveau avec le contenu correspondant dans le fichier. Assigne la Position du joueur quand il est trouve. Empile les Cible trouvees dans pileCible.
 *
 * \param path Nom du fichier contenant les niveaux.
 * \param niveau Pointeur sur le Niveau a remplir.
 * \param numero Numero du niveau a charger.
 * \param largeurNiveau Pointeur sur l'entier qui contiendra la largeur du niveau.
 * \param hauteurNiveau Pointeur sur l'entier qui contiendra la hauteur du niveau.
 * \param posJoueur Pointeur sur la Position du joueur, assignee des que le joueur est trouve dans le fichier.
 * \param pileCible Pointeur sur une pile de Cible, chaque Cible trouvee dans le niveau y est empilee.
 *
 * \return Renvoie UndefinedLevel si niveau est nul, UndefinedParameter si un des autres parametres est nul, FichierIntrouvable si le fichier de niveaux n'existe pas, AllocationMemoire si erreur d'allocation du Niveau, CaractereInconnu si un caractere inconnu est present dans le Niveau, NoError sinon.
 */
LevelError readLevel(char* path, Niveau* niveau, unsigned int numero, unsigned int* largeurNiveau, unsigned int* hauteurNiveau, Position* posJoueur, PileCible* pileCible) {
	LevelError code_e = NoError;
	fpos_t positionDansFichier;
	FILE* levels = NULL;

	if(path == NULL || largeurNiveau == NULL || hauteurNiveau == NULL || posJoueur == NULL || pileCible == NULL) {
		return UndefinedParameter;
	}
	if(niveau == NULL) {
		return UndefinedLevel;
	}

	*hauteurNiveau = 0;
	*largeurNiveau = 0;
	
	/* Ouverture du fichier */
	levels = fopen(path, "r");
	if(levels == NULL) {
		fprintf(stderr, "Fichier de niveaux introuvable : %s.\n", path);
		fclose(levels);
		return FichierIntrouvable;
	}
	
	/* Recuperation de la taille du niveau */
	getTailleNiveau(levels, numero, largeurNiveau, hauteurNiveau, &positionDansFichier);

	code_e = alloueNiveau(niveau, *largeurNiveau, *hauteurNiveau);
	if(code_e != NoError) {
		fprintf(stderr, "(%d) Erreur lors de l'allocation memoire.\n", code_e);
		fclose(levels);
		return AllocationMemoire;
	}

	code_e = remplirNiveau(levels, &positionDansFichier, niveau, *largeurNiveau, *hauteurNiveau, posJoueur, pileCible);
	if(code_e != NoError) {
		fprintf(stderr, "(%d) Caractere inconnu present dans le niveau %d.\n", code_e, numero);
		libererPileCible(pileCible);
		freeNiveau(niveau, *hauteurNiveau);
		fclose(levels);
		return CaractereInconnu;
	}

	/* Fermeture du fichier */
	fclose(levels);
	return NoError;
}

/**
 * \fn LevelError updateLevel(UpdateType operation, char* fichierNiveaux, Niveau* niveau, Position* posJoueur, PileCible* pileCible, unsigned int* numLvl, unsigned int* lNiveau, unsigned int* hNiveau)
 *
 * \brief Met a jour le Niveau designe par niveau : le reinitialiser, charger le niveau suivant, charger le niveau precedent.
 *
 * \param operation Type de l'operation, aller au niveau precedent, suivant, ou reinitialiser celui en cours.
 * \param fichierNiveaux Nom du fichier contenant les niveaux.
 * \param niveau Pointeur sur le Niveau a remplir.
 * \param posJoueur Pointeur sur la Position du joueur, assignee des que le joueur est trouve dans le fichier.
 * \param pileCible Pointeur sur une pile de Cible, chaque Cible trouvee dans le niveau y est empilee.
 * \param numLvl Pointeur sur la variable contenant le numero du niveau actuel.
 * \param lNiveau Pointeur sur l'entier qui contiendra la largeur du niveau.
 * \param hNiveau Pointeur sur l'entier qui contiendra la hauteur du niveau.
 * \return Renvoie UndefinedLevel si niveau est nul, UndefinedParameter si un des autres parametres est nul, FichierIntrouvable si le fichier de niveaux n'existe pas, AllocationMemoire si erreur d'allocation du Niveau, CaractereInconnu si un caractere inconnu est present dans le Niveau, NoError sinon.
 */
LevelError updateLevel(UpdateType operation, char* fichierNiveaux, Niveau* niveau, Position* posJoueur, PileCible* pileCible, unsigned int* numLvl, unsigned int* lNiveau, unsigned int* hNiveau) {

	if(fichierNiveaux == NULL || posJoueur == NULL || pileCible == NULL || numLvl == NULL || lNiveau == NULL || hNiveau == NULL) {
		return UndefinedParameter;
	}

	if(niveau == NULL) {
		return UndefinedLevel;
	}

	if(operation == LevelPrevious) {
		(*numLvl)--;
		if ((*numLvl) < 1) {
			(*numLvl) = 1;
		}
	}

	if(operation == LevelNext) {
		(*numLvl)++;
		if ((*numLvl) > MAX_LEVEL) {
			(*numLvl) = MAX_LEVEL;
		}
	}

	libererPileCible(pileCible);
	freeNiveau(niveau, *hNiveau);
	return readLevel(fichierNiveaux, niveau, *numLvl, lNiveau, hNiveau, posJoueur, pileCible);
}

/**
 * \fn LevelError createLevel(Niveau* niveau, unsigned int* largeurNiveau, unsigned int* hauteurNiveau)
 *
 * \brief Cree un nouveau Niveau, rempli de SOL, de taille NB_BLOCS_HAUTEUR x NB_BLOCS_LARGEUR. 
 *
 * \param niveau Pointeur sur le Niveau a remplir.
 * \param largeurNiveau Pointeur sur l'entier qui contiendra la largeur du niveau.
 * \param hauteurNiveau Pointeur sur l'entier qui contiendra la hauteur du niveau.
 * \return Renvoie UndefinedLevel si niveau est nul, UndefinedParameter si un des autres parametres est nul, AllocationMemoire si erreur d'allocation du Niveau, NoError sinon.
 */
LevelError createLevel(Niveau* niveau, unsigned int* largeurNiveau, unsigned int* hauteurNiveau) {
	LevelError code_e = NoError;

	if(largeurNiveau == NULL || hauteurNiveau == NULL) {
		return UndefinedParameter;
	}
	if(niveau == NULL) {
		return UndefinedLevel;
	}

	*hauteurNiveau = NB_BLOCS_HAUTEUR;
	*largeurNiveau = NB_BLOCS_LARGEUR;

	code_e = alloueNiveau(niveau, *largeurNiveau, *hauteurNiveau);
	if(code_e != NoError) {
		fprintf(stderr, "(%d) Erreur lors de l'allocation memoire.\n", code_e);
		return AllocationMemoire;
	}

	return NoError;
}
