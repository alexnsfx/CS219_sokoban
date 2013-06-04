#include "mouvement.h"

PileError empilerCoup(CoupsJoues* pile, Direction direction, Position posDepart, char caisseDeplacee) {
	if(pile == NULL) {
		return UndefinedPile;
	}
	
	Coup* nouveauCoup = (Coup*)malloc(sizeof(Coup));
	if(nouveauCoup == NULL) {
		return AllocError;
	}
	
	nouveauCoup->direction = direction;
	nouveauCoup->posDepart = posDepart;
	nouveauCoup->caisseDeplacee = caisseDeplacee;
	nouveauCoup->next = *pile;
	*pile = nouveauCoup;
	
	return Ok;
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
 * \fn void deplacerObjet(Niveau* n, Coup* mouvement , int* nbMvt, int* nbPoussee)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param n Pointeur sur le fichier de niveaux
 * \param mouvement Deplacement souhaite 
 * \param nbMvt Compteur de mouvements
 * \param nbPoussee Compteur de poussees de caisses
 */
void deplacerObjet(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee) {
	Position position, positionCaisseFin;
	position = (*mouvement).posDepart;
	positionCaisseFin = (*mouvement).posDepart;

	calculPosition(&position, (*mouvement).direction, Positif);

	if ((*mouvement).caisseDeplacee == 1 ) {	
		calculPosition(&positionCaisseFin, (*mouvement).direction, Positif);
		calculPosition(&positionCaisseFin, (*mouvement).direction, Positif);
		majNiveau(position, positionCaisseFin, n);
		(*nbPoussee)++;
	}

	majNiveau((*mouvement).posDepart, position, n);
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
	Position position = (*mouvement).posDepart;

	calculPosition(&position, (*mouvement).direction, Negatif);
	majNiveau((*mouvement).posDepart, position, n);
	(*nbMvt)--;

	if ((*mouvement).caisseDeplacee == 1) {		
		calculPosition(&position, (*mouvement).direction, Negatif);
		calculPosition(&position, (*mouvement).direction, Negatif);
		majNiveau(position, (*mouvement).posDepart, n);
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
void majNiveau(Position positionDepart, Position position, Niveau* n) {
	switch ((*n)[position.y][position.x]) {
				case CIBLE:	switch ((*n)[positionDepart.y][positionDepart.x]) {
								case JOUEUR:
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[position.y][position.x] = JOUEUR_CIBLE;
									break;
								case CAISSE:
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[position.y][position.x] = CAISSE_CIBLE;	
									break;	
								case CAISSE_CIBLE:
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[position.y][position.x] = CAISSE_CIBLE;		
									break;	
								case JOUEUR_CIBLE:	
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[position.y][position.x] = JOUEUR_CIBLE;	
									break;	
								default:	break;	/* erreur, l'objet en position de depart n'est pas censé se deplacer */
							}
				case SOL:	switch ((*n)[positionDepart.y][positionDepart.x]) {
								case JOUEUR:	
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[position.y][position.x] = JOUEUR;	
									break;
								case CAISSE:		
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[position.y][position.x] = CAISSE;	
									break;	
								case CAISSE_CIBLE:		
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[position.y][position.x] = CAISSE;	
									break;	
								case JOUEUR_CIBLE:
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[position.y][position.x] = JOUEUR;		
									break;	
								default:	break;	/* erreur, l'objet en position de depart n'est pas censé se deplacer */
							}
				default :	break; 	/* erreur, on ne peut pas marcher sur la case d'arrivee */
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
TypeDeplacement deplacementPossible(Niveau* n, Position position, int liberte, Direction direction) {
	Position pos = position;
	calculPosition(&pos, direction, Positif);

	switch ((*n)[pos.y][pos.x]) {
				case MUR:
				case JOUEUR:
				case JOUEUR_CIBLE:
					return Impossible; /* la case est occupee, l'objet ne peut pas se deplacer */
				case CAISSE:	
				case CAISSE_CIBLE:	
					if(liberte == 0) {	/* on re-appelle la fonction pour voir si la caisse rencontree peut se deplacer */
						if(deplacementPossible(n, pos, 1, direction) < Impossible) {
							return PossibleAvecCaisse;
						}
						else {
							return Impossible;
						}
					}
					else {	/* test du deplacement de la caisse */
						return Impossible;	/* si elle ne peut pas se deplacer */
					}
				default:
					return Possible;	/* caractere autre (sol, cible), deplacement sans caisse */
	}
}

/**
 * \fn void calculPositionPositif (int* ordonnee, int* abscisse, Coup* mouvement)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param ordonne Ordonnee d'origine
 * \param abscisse Abscisse d'origine 
 * \param mouvement  Mouvement demande
 */
void calculPosition(Position* position, Direction direction, SensDeplacement sens) {
	switch (direction) {
				case HAUT:	(position->y) -= sens;
						break;
				case BAS:	(position->y) += sens;
						break;	
				case GAUCHE:	(position->x) -= sens;
						break;	
				case DROITE:	(position->x) += sens;
						break;	
				default:	break;	/* erreur, direction invalide */
	}	
}