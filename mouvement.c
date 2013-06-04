#include "mouvement.h"

/* 0:ok, 1:erreurAlloc, 2:pile* nul */
char empilerCoup(CoupsJoues* pile, Direction direction, int posDepartOrdonnee, int posDepartAbscisse, char caisseDeplacee) {
	if(pile == NULL) {
		return 2;
	}
	
	Coup* nouveauCoup = (Coup*)malloc(sizeof(Coup));
	if(nouveauCoup == NULL) {
		return 1;
	}
	
	nouveauCoup->direction = direction;
	nouveauCoup->posDepartOrdonnee = posDepartOrdonnee;
	nouveauCoup->posDepartAbscisse = posDepartAbscisse;
	nouveauCoup->caisseDeplacee = caisseDeplacee;
	nouveauCoup->next = *pile;
	*pile = nouveauCoup;
	
	return 0;
}

Coup* depilerCoup(CoupsJoues* pile) {
	if(pile == NULL || *pile == NULL) {
		return NULL;
	}
	
	Coup* tete = *pile;
	*pile = (*pile)->next;
	return tete;
}

void libererPile(CoupsJoues* pile) {
	if(pile == NULL || *pile == NULL) {
		return;
	}

	if((*pile)->next != NULL) {
		libererPile(&((*pile)->next));
		(*pile)->next = NULL;
	}
	free(*pile);
	*pile = NULL;
}



/**
 * \fn void calculPositionPositif (int* ordonnee, int* abscisse, Coup* mouvement)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param ordonne Ordonnee d'origine
 * \param abscisse Abscisse d'origine 
 * \param mouvement  Mouvement demande
 */

void calculPositionPositif (int* ordonnee, int* abscisse, Coup* mouvement) {
	switch ((*mouvement).direction) {
				case HAUT:	(*ordonnee)--;
						break;
				case BAS:	(*ordonnee)++;	
						break;	
				case GAUCHE:	(*abscisse)--;	
						break;	
				case DROITE:	(*abscisse)++;
						break;	
				default:	break;	/* erreur */
	}	
}
/**
 * \fn void calculPositionNegatif (int* ordonnee, int* abscisse, Coup* mouvement)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param ordonne Ordonnee d'origine
 * \param abscisse Abscisse d'origine 
 * \param mouvement  Mouvement demande
 */

void calculPositionNegatif (int* ordonnee, int* abscisse, Coup* mouvement) {
	switch ((*mouvement).direction) {
				case HAUT:	ordonnee++;
						break;
				case BAS:	ordonnee--;	
						break;	
				case GAUCHE:	abscisse++;	
						break;	
				case DROITE:	abscisse--;
						break;	
				default:	break;	/* erreur */
	}
}

/**
 * \fn void deplacerObjet(Niveau* n, Coup* mouvement , int* nbMvt, int* nbPoussee)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param n Pointeur sur le fichier de niveaux
 * \param mouvement Deplacement souhaite 
 * \param nbMvt Compteur de mouvements
 * \param nbPoussee Compteur de poussees de caisses
 */
void deplacerObjet(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee) {
	int ordonneeCaisseFin, ordonnee;
	int abscisseCaisseFin, abscisse;

	ordonneeCaisseFin = (*mouvement).posDepartOrdonnee;
	ordonnee = (*mouvement).posDepartOrdonnee;

	abscisseCaisseFin = (*mouvement).posDepartAbscisse;
	abscisse = (*mouvement).posDepartAbscisse;

	calculPositionPositif (&ordonnee, &abscisse, mouvement);

	if ((*mouvement).caisseDeplacee == 1 ) {	
		calculPositionPositif (&ordonneeCaisseFin, &abscisseCaisseFin, mouvement);
		calculPositionPositif (&ordonneeCaisseFin, &abscisseCaisseFin, mouvement);
		majNiveau(ordonnee, abscisse, ordonneeCaisseFin, abscisseCaisseFin, n);
		(*nbPoussee)++;
	}

	majNiveau((*mouvement).posDepartOrdonnee,(*mouvement).posDepartAbscisse, ordonnee, abscisse, n);
	(*nbMvt)++;
}

/**
 * \fn void deplacerObjetRetour(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee);
 * \brief Redeplace l'objet comme il etait un coup avant
 *
 * \param n Pointeur sur le fichier de niveaux
 * \param mouvement Deplacement souhaite 
 * \param nbMvt Compteur de mouvements
 * \param nbPoussee Compteur de poussees de caisses
 */
void deplacerObjetRetour(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee) {
	int ordonnee = (*mouvement).posDepartOrdonnee;
	int abscisse = (*mouvement).posDepartAbscisse;

	calculPositionNegatif (&ordonnee, &abscisse, mouvement);
	majNiveau((*mouvement).posDepartOrdonnee,(*mouvement).posDepartAbscisse, ordonnee, abscisse, n);
	(*nbMvt)--;

	if ((*mouvement).caisseDeplacee == 1) {		
	calculPositionPositif (&ordonnee, &abscisse, mouvement);
	calculPositionPositif (&ordonnee, &abscisse, mouvement);
	majNiveau(ordonnee, abscisse,(*mouvement).posDepartOrdonnee,(*mouvement).posDepartAbscisse, n);
	(*nbPoussee)--;
	}
}

/**
 * \fn void majNiveau(int ordonneeInit, int abscisseInit, int ordonnee, int abscisse, Niveau* n);
 * \brief Remplie le tableau avec les bons caracteres
 *
 * \param ordonneeInit Ordonnee de l'objet d'origine
 * \param abscisseInit  Abscisse de l'objet d'origine
 * \param ordonnee Ordonnee de l'objet apres deplacement
 * \param abscisse Abscisse de l'objet apres deplacement
 * \param n Pointeur sur le fichier de niveaux
 */
void majNiveau(int ordonneeInit, int abscisseInit, int ordonnee, int abscisse, Niveau* n) {
	switch ((*n)[ordonnee][abscisse]) {
				case CIBLE:	switch ((*n)[ordonneeInit][abscisseInit]) {
								case JOUEUR:
									(*n)[ordonneeInit][abscisseInit] = SOL;
									(*n)[ordonnee][abscisse] = JOUEUR_CIBLE;
									break;
								case CAISSE:
									(*n)[ordonneeInit][abscisseInit] = SOL;
									(*n)[ordonnee][abscisse] = CAISSE_CIBLE;	
									break;	
								case CAISSE_CIBLE:
									(*n)[ordonneeInit][abscisseInit] = CIBLE;
									(*n)[ordonnee][abscisse] = CAISSE_CIBLE;		
									break;	
								case JOUEUR_CIBLE:	
									(*n)[ordonneeInit][abscisseInit] = CIBLE;
									(*n)[ordonnee][abscisse] = JOUEUR_CIBLE;	
									break;	
								default:	break;	
							}
				case SOL:	switch ((*n)[ordonneeInit][abscisseInit]) {
								case JOUEUR:	
									(*n)[ordonneeInit][abscisseInit] = SOL;
									(*n)[ordonnee][abscisse] = JOUEUR;	
									break;
								case CAISSE:		
									(*n)[ordonneeInit][abscisseInit] = SOL;
									(*n)[ordonnee][abscisse] = CAISSE;	
									break;	
								case CAISSE_CIBLE:		
									(*n)[ordonneeInit][abscisseInit] = CIBLE;
									(*n)[ordonnee][abscisse] = CAISSE;	
									break;	
								case JOUEUR_CIBLE:
									(*n)[ordonneeInit][abscisseInit] = CIBLE;
									(*n)[ordonnee][abscisse] = JOUEUR;		
									break;	
								default:	break;	/*  erreur */
							}
				default :	break; 	/* erreur */
				}
}

	
/**
 * \fn void annulerCoup(Coup* mouvement)
 * \brief Annule la derniere action en redeplacant l'objet comme il etait un coup avant
 *
 * \param Mouvement Deplacement souhaite
 */
void annulerCoup(Niveau* n, CoupsJoues* pile, int* nbMvt, int* nbPoussee) {
	Coup* tete = depilerCoup(pile);
	deplacerObjetRetour(n, tete, nbMvt, nbPoussee);
	free(tete);
}

/**
 * \fn char deplacementPossible(int abscisse, int ordonnee, int liberte);
 * \brief Teste si le deplacement vers la direction souhaitee est possible
 *
 * \param n Pointeur sur le fichier de niveaux
 * \param abscisse Position en abscisse de l'objet a deplacer
 * \param ordonnee Position en ordonnee de l'objet a deplacer
 * \param liberte Limiter a un niveau de recurrence
 * \param direction Donne la direction du deplacement
 * \return Renvoie un code d'erreur, 0 si le deplacement est possible
 */
char deplacementPossible(Niveau* n, int ordonnee, int abscisse, int liberte, Direction direction) {
	int ordo = ordonnee;
	int absc = abscisse;
	switch (direction) {
				case HAUT:	ordo--;
						break;	
				case BAS:	ordo++;	
						break;	
				case GAUCHE:	absc--;	
						break;	
				case DROITE:	absc++;
						break;	
				default:	return 2;	/* direction inconnue */
	}

	switch ((*n)[ordo][absc]) {
				case MUR:
				case JOUEUR:
				case JOUEUR_CIBLE:
					return 1; /* l'objet ne peut pas se deplacer*/
				case CAISSE:	
				case CAISSE_CIBLE:	
					if(liberte == 0) {	/* on re-appelle la fonction pour voir si la caisse */
								/* rencontree peut se deplacer */ 
						return deplacementPossible(n, ordo, absc, 1, direction);
					}
					else {	/* test du deplacement de la caisse */
						return 1;	/* si elle ne peut pas se deplacer */
					}
				default:	return 0;	/* caractere autre (sol, cible) */
	}
}