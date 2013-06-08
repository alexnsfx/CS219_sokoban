#include "mouvement.h"

/**
 * \fn PileError empilerCoup(CoupsJoues* pile, Direction direction, Position posDepart, char caisseDeplacee)
 * \brief Empile les coups 
 *
 * \param pile Pile a depiler
 * \direction Direction du coup
 * \posDepart Position avant le coup
 * \caisseDeplacee 1 Si une caisse a ete deplacee, 0 si non
 *
 * \return Renvoie UndefinedPile si l'une des piles n'existe pas, p_error lors d'un probleme d'inversion, Ok lorsque la fonction s'est executee correctement
 */
PileError empilerCoup(CoupsJoues* pile, Direction direction, Position posDepart, char caisseDeplacee) {
	Coup* nouveauCoup = NULL;	
	
	if(pile == NULL) {
		return UndefinedPile;
	}
	
	nouveauCoup = (Coup*)malloc(sizeof(Coup));
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

/**
 * \fn Coup* depilerCoup(CoupsJoues* pile)
 * \brief Depile la pile de coup et renvoie le coup depile
 *
 * \param pile Pile a depiler
 *
 * \return Renvoie NULL si la pile n'existe pas, ou le coup depile
 */
Coup* depilerCoup(CoupsJoues* pile) {
	Coup* tete = NULL;	
	if(pile == NULL || *pile == NULL) {
		return NULL;
	}
	
	tete = *pile;
	*pile = (*pile)->next;
	return tete;
}

/**
 * \fn void libererPile(CoupsJoues* pile)
 * \brief Libere la pile
 *
 * \param pile Pile a liberer
 */
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
 * \fn PileError inverserPileCoups(CoupsJoues* pile, CoupsJoues* pileInversee)
 * \brief Inverse l'ordre de la pile
 *
 * \param pile Pile d'origine
 * \param pileInversee  Pile inversee (apres execution de la fonction)
 *
 * \return Renvoie UndefinedPile si l'une des piles n'existe pas, p_error lors d'un probleme d'inversion, Ok lorsque la fonction s'est executee correctement
 */
PileError inverserPileCoups(CoupsJoues* pile, CoupsJoues* pileInversee) {
	PileError p_error = Ok;

	if(pile == NULL || pileInversee == NULL) {
		return UndefinedPile;
	}

	while(*pile != NULL) {
		p_error = empilerCoup(pileInversee, (*pile)->direction, (*pile)->posDepart, (*pile)->caisseDeplacee);
		if(p_error != Ok) {
			fprintf(stderr, "Erreur lors de l'inversion de la pile de coups.\n");
			return p_error;
		}

		*pile = (*pile)->next;
	}

	return Ok;
}

/**
 * \fn void deplacerObjet(Niveau* n, Coup mouvement, unsigned int* nbMvt, unsigned int* nbPoussee)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param n Pointeur sur le tableau de niveaux
 * \param mouvement Deplacement souhaite 
 * \param nbMvt Compteur de mouvements
 * \param nbPoussee Compteur de poussees de caisses
 */
void deplacerObjet(Niveau* n, Coup mouvement, unsigned int* nbMvt, unsigned int* nbPoussee) {
	Position position, positionCaisseFin;

	if(n == NULL || nbMvt == NULL || nbPoussee == NULL) {
		return;
	}

	position = mouvement.posDepart;
	positionCaisseFin = mouvement.posDepart;

	calculPosition(&position, mouvement.direction, Positif);

	if (mouvement.caisseDeplacee == 1) {	
		calculPosition(&positionCaisseFin, mouvement.direction, Positif);
		calculPosition(&positionCaisseFin, mouvement.direction, Positif);
		majNiveau(position, positionCaisseFin, n);
		(*nbPoussee)++;
	}

	majNiveau(mouvement.posDepart, position, n);
	(*nbMvt)++;
}

/**
 * \fn void deplacerObjetRetour(Niveau* n, Coup mouvement, unsigned int* nbMvt, unsigned int* nbPoussee)
 * \brief Redeplace l'objet comme il etait un coup avant
 *
 * \param n Pointeur sur le tableau de niveaux
 * \param mouvement Deplacement souhaite 
 * \param nbMvt Compteur de mouvements
 * \param nbPoussee Compteur de poussees de caisses
 */
void deplacerObjetRetour(Niveau* n, Coup mouvement, unsigned int* nbMvt, unsigned int* nbPoussee) {
	Position positionDepart = mouvement.posDepart;
	Position positionDepartCaisse = mouvement.posDepart;

	if(n == NULL || nbMvt == NULL || nbPoussee == NULL) {
		return;
	}

	calculPosition(&positionDepart, mouvement.direction, Positif);
	majNiveau(positionDepart, mouvement.posDepart, n);
	(*nbMvt)--;

	if (mouvement.caisseDeplacee == 1) {		
		calculPosition(&positionDepartCaisse, mouvement.direction, Positif);
		calculPosition(&positionDepartCaisse, mouvement.direction, Positif);
		majNiveau(positionDepartCaisse, positionDepart, n);
		(*nbPoussee)--;
	}
}

/**
 * \fn void majNiveau(Position positionDepart, Position positionFin, Niveau* n) 
 * \brief Remplie le tableau avec les bons caracteres
 *
 * \param positionDepart Position de l'objet avant deplacementcement
 * \param positionFin Position de l'objet apres deplacement
 * \param n Pointeur sur le tableau de niveaux
 */
void majNiveau(Position positionDepart, Position positionFin, Niveau* n) {
	if(n == NULL) {
		return;
	}

	switch ((*n)[positionFin.y][positionFin.x]) {
				case CIBLE:	switch ((*n)[positionDepart.y][positionDepart.x]) {
								case JOUEUR:
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[positionFin.y][positionFin.x] = JOUEUR_CIBLE;
									break;
								case CAISSE:
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[positionFin.y][positionFin.x] = CAISSE_CIBLE;	
									break;	
								case CAISSE_CIBLE:
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[positionFin.y][positionFin.x] = CAISSE_CIBLE;		
									break;	
								case JOUEUR_CIBLE:	
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[positionFin.y][positionFin.x] = JOUEUR_CIBLE;	
									break;	
								default:
									break;	/* erreur, l'objet en position de depart n'est pas censé se deplacer */
						}
						break;
				case SOL:	switch ((*n)[positionDepart.y][positionDepart.x]) {
								case JOUEUR:	
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[positionFin.y][positionFin.x] = JOUEUR;	
									break;
								case CAISSE:		
									(*n)[positionDepart.y][positionDepart.x] = SOL;
									(*n)[positionFin.y][positionFin.x] = CAISSE;	
									break;	
								case CAISSE_CIBLE:		
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[positionFin.y][positionFin.x] = CAISSE;	
									break;	
								case JOUEUR_CIBLE:
									(*n)[positionDepart.y][positionDepart.x] = CIBLE;
									(*n)[positionFin.y][positionFin.x] = JOUEUR;		
									break;	
								default:
									break;	/* erreur, l'objet en position de depart n'est pas censé se deplacer */
						}
						break;
				default :	
					break; 	/* erreur, on ne peut pas marcher sur la case d'arrivee */
	}
}

/**
 * \fn TypeDeplacement deplacementPossible(Niveau* n, Position position, char liberte, Direction direction)
 * \brief Teste si le deplacement vers la direction souhaitee est possible
 *
 * \param n Pointeur sur le tableau de niveaux
 * \param position Position de l'objet a deplacer
 * \param liberte Limiter a un niveau de recurrence
 * \param direction Donne la direction du deplacement
 *
 * \return Renvoie un code d'erreur pour savoir si le deplacement est possible
 */
TypeDeplacement deplacementPossible(Niveau* n, Position position, char liberte, Direction direction) {
	Position pos = position;

	if(n == NULL) {
		return Impossible;
	}

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
 * \fn void calculPosition(Position* position, Direction direction, SensDeplacement sens)
 * \brief Calcule la position apres deplacement
 *
 * \param position Position initiale avant deplacement
 * \param direction Direction du deplacement
 * \param sens  Positif pour un coup normal, negatif pour un retour en arriere
 */
void calculPosition(Position* position, Direction direction, SensDeplacement sens) {
	switch (direction) {
				case HAUT:	position->y -= sens;
						break;
				case BAS:	position->y += sens;
						break;	
				case GAUCHE:	position->x -= sens;
						break;	
				case DROITE:	position->x += sens;
						break;	
				default:	break;	/* erreur, direction invalide */
	}	
}

void arrowKeyDownHandler(Direction dir, Niveau* n, Position* posJoueur, CoupsJoues* pile, unsigned int* nbMvt, unsigned int* nbPoussee) {
	TypeDeplacement deplacementCaisse = Impossible;

	if(n == NULL || posJoueur == NULL || pile == NULL || nbMvt == NULL || nbPoussee == NULL) {
		return;
	}

	deplacementCaisse = deplacementPossible(n, *posJoueur, 0, dir);
	if(deplacementCaisse < Impossible) {
		if(empilerCoup(pile, dir, *posJoueur, deplacementCaisse) != Ok) {
			return;
		}
		deplacerObjet(n, **pile, nbMvt, nbPoussee);
		calculPosition(posJoueur, dir, Positif);
	}
}
