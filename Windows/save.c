#include "save.h"

PileSaveLoadError chargerPileCoups(CoupsJoues* pile, FILE* fichier) {
	PileError p_error = Ok;
	char ligne[SAVE_BUFFER_WIDTH] = {0};
	
	int direction;
	Position posDepart;
	int caisseDeplacee;

	if(pile == NULL) {
		return SaveLoadUndefinedPile;
	}
	if(fichier == NULL) {
		return SaveLoadUndefinedFile;
	}

	/* Lit les coups ligne par ligne */
	while (fgets(ligne, SAVE_BUFFER_WIDTH, fichier) != NULL) {
		if (sscanf(ligne, "%d;%d;%d;%d\n", &direction, &(posDepart.x), &(posDepart.y), &caisseDeplacee) == 4) {
			/* si un coup est lu, on l'empile */
			p_error = empilerCoup(pile, (Direction)direction, posDepart, (char)(caisseDeplacee&0xFF));
			if(p_error != Ok) {
				fprintf(stderr, "Erreur lors de la lecture des coups.\n");
				return SaveLoadEmpileError;
			}
		}
	}

	return SaveLoadNoError;
}

PileSaveLoadError dechargerPileCoups(CoupsJoues* pile, FILE* fichier) {
	Coup* curseur = *pile;
	if(pile == NULL) {
		return SaveLoadUndefinedPile;
	}
	if(fichier == NULL) {
		return SaveLoadUndefinedFile;	
	}

	/* Pour chaque coup de la pile, on l'ecrit dans le fichier */
	while(curseur != NULL) {
		fprintf(fichier, "%d;%d;%d;%d\n", curseur->direction, curseur->posDepart.x, curseur->posDepart.y, curseur->caisseDeplacee);

		curseur = curseur->next;
	}

	return SaveLoadNoError;
}

SaveFileError creerFichierSauvegarde(SaveType sType, int numero, CoupsJoues* pile) {
	FILE* fichierSauvegarde = NULL;
	char chemin[28] = {0};

	/* Change l'extension du fichier de sauvegarde selon le type de sauvegarde : partie ou solution */
	sprintf(chemin, "./save/level%d.%s", numero, (sType == Sauvegarde) ? "save" : "sol");

	/* Cree le fichier */
	fichierSauvegarde = fopen(chemin, "w");
	if(fichierSauvegarde == NULL) {
		fprintf(stderr, "(%d) Impossible de creer le fichier de sauvegarde du niveau %d.\n", CreateError, numero);
		return CreateError;
	}

	/* Enregistre la pile de coups dans le fichier */
	dechargerPileCoups(pile, fichierSauvegarde);

	fclose(fichierSauvegarde);

	return CreateOk;
}

LoadFileError chargerFichierSauvegarde(unsigned int numero, CoupsJoues* pileCoups, Niveau* n, Position* pos, PileCible* cibles, unsigned int* nbMvt, unsigned int* nbPoussee) {
	LevelError level_e = NoError;
	PileError pile_e = Ok;
	PileSaveLoadError psl_e = SaveLoadNoError;

	Coup* curseur = NULL;
	char chemin[28] = {0};
	FILE* fichierSauvegarde = NULL;
	CoupsJoues pileDeCoupsTmp = NULL;
	unsigned int tmph = 0;
	unsigned int tmpl = 0;
	unsigned int num_tmp = numero;

	if(pileCoups == NULL || n == NULL || pos == NULL || cibles == NULL || nbMvt == NULL || nbPoussee == NULL) {
		return LoadUndefinedParameter;
	}

	sprintf(chemin, "./save/level%d.save", numero);

	/* reinitialise le niveau */
	libererPile(pileCoups);
	level_e = updateLevel(LevelReset, FICHIER_NIVEAUX, n, pos, cibles, &num_tmp, &tmph, &tmpl);
	if(level_e != NoError) {
		fprintf(stderr, "(%d) Erreur lors de la reinitialisation du niveau.\n", level_e);
		return LoadError;
	}

	*nbMvt = 0;
	*nbPoussee = 0;

	/* ouvre le fichier de sauvegarde */
	fichierSauvegarde = fopen(chemin, "r");
	if(fichierSauvegarde == NULL) {
		fprintf(stderr, "Impossible d'ouvrir le fichier \"%s\".\n", chemin);
		return LoadError;
	}

	/* on charge les coups */
	psl_e = chargerPileCoups(pileCoups, fichierSauvegarde);
	if(psl_e != SaveLoadNoError) {
		fprintf(stderr, "(%d) Erreur lors du chargement de la pile de coups.\n", psl_e);
		return LoadError;
	}

	/* on joue les coups */
	curseur = *pileCoups;
	while(curseur != NULL) {
		deplacerObjet(n, *curseur, nbMvt, nbPoussee);
		calculPosition(pos, curseur->direction, Positif);
		curseur = curseur->next;
	}
	fclose(fichierSauvegarde);

	/* nous avons stocke les coups dans une pile, il faut donc l'inverser pour empiler les prochains coups au bon endroit */
	pile_e = inverserPileCoups(pileCoups, &pileDeCoupsTmp);
	if(pile_e != Ok) {
		fprintf(stderr, "(%d) Erreur lors de l'inversion de la pile de coups.\n", pile_e);
		return LoadError;
	}

	libererPile(pileCoups);
	*pileCoups = pileDeCoupsTmp;

	return LoadOk;
}

EditionError editeNiveau(char* fichierLevel, char* fichierTemp, Niveau* niveau, int numLvl, int hauteur) {
	FILE *fLevel, *fTemp;
	char ligne[BUFFER_WIDTH] = {0};
	int i = 0;
	char copie = 0;
	int numero_courant = 0;

	if(fichierLevel == NULL || fichierTemp == NULL) {
		fprintf(stderr, "(%d) Chemin de fichier non defini.\n", EditionUndefinedPath);
		return EditionUndefinedPath;
	}
	
	if(niveau == NULL) {
		fprintf(stderr, "(%d) Niveau non defini.\n", EditionUndefinedLevel);
		return EditionUndefinedLevel;
	}

	fLevel = fopen(fichierLevel, "r");
	if(fLevel == NULL) {
		fprintf(stderr, "(%d) Impossible d'ouvrir le fichier \"%s\".\n", EditionOpenFileError, fichierLevel);
		return EditionOpenFileError;
	}

	fTemp = fopen(fichierTemp, "w");
	if(fTemp == NULL) {
		fprintf(stderr, "(%d) Impossible d'ouvrir le fichier \"%s\".\n", EditionOpenFileError, fichierTemp);
		fclose(fLevel);
		return EditionOpenFileError;
	}

	/* Copie fichierLevel dans fichierTemp jusqu'au debut du niveau  */
	while(fgets(ligne, BUFFER_WIDTH, fLevel) != NULL) {
		if (sscanf(ligne, ";LEVEL %d", &numero_courant) == 1 && numero_courant == numLvl) {
			
			/* une fois le niveau atteint, on l'ecrit dans fichierTemp */
			fprintf(fTemp, ";LEVEL %d\n", numLvl);
			for(i = 0; i < hauteur; i++) {
				fprintf(fTemp, "%s\n", (*niveau)[i]);
			}
			break;
		}

		fprintf(fTemp, "%s", ligne);
	}

	/* on parcourt fichierLevel jusqu'au prochain niveau, sans copier les lignes */
	while(fgets(ligne, BUFFER_WIDTH, fLevel) != NULL) {
		if (sscanf(ligne, ";LEVEL %d", &numero_courant) == 1 && numero_courant == (numLvl+1)) {
			copie = 1;	
		}
		if(copie) {
			/* une fois le prochain niveau atteint dans fichierLevel, */
			/* on reprend la copie des niveaux suivant depuis fichierLevel dans fichierTemp */
			fprintf(fTemp, "%s", ligne);
		}
	}
	
	fclose(fLevel);
	fclose(fTemp);

	remove(fichierLevel);
	rename(fichierTemp, fichierLevel);

	return EditionNoError;
}
