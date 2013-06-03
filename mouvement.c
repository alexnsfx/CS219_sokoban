/**
 * \fn void deplacerObjet(Niveau* n, Coup* mouvement , int* nbMvt, int* nbPoussee)
 * \brief Deplace un objet dans la direction souhaitee
 *
 * \param n Pointeur sur le fichier de niveaux
 * \param mouvement Deplacement souhaite 
 * \param nbMvt Compteur de mouvements
 * \param nbPoussee Compteur de poussees de caisses
 */
void deplacerObjet(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee, int liberte) {
	int ordonnee = (*mouvement).posDepartOrdonnee;
	int abscisse = (*mouvement).posDepartAbscisse;

	switch (Direction) {
		case HAUT:	ordonnee--;
					break;
		case BAS:	ordonnee++;	
					break;	
		case GAUCHE:	abscisse--;	
						break;	
		case DROITE:	abscisse++;
						break;	
		default:	return 2;	/*  erreur */
	}
	
	if (caisseDeplacee == 1 && liberte == 0) {	/* si on doit deplacer la caisse */
		*nbPoussee++;								/* re-execution de la fonction */
		deplacerObjet(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee, 1);	/* avec la caisse en objet */
	}
	
	majNiveau((*mouvement).posDepartOrdonnee,(*mouvement).posDepartAbscisse, ordonnee, abscisse, n);
	*nbMvt++;
	
	if (caisseDeplacee == 1) {
		*nbPoussee++;
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
	switch (n[ordonnee][abscisse]) 
				case CIBLE:	switch (n[ordonneeInit][abscisseInit]) {
							case JOUEUR:
								n[ordonneeInit][abscisseInit] = SOL;
								n[ordonnee][abscisse] = JOUEUR_CIBLE;
								break;
							case CAISSE:
								n[ordonneeInit][abscisseInit] = SOL;
								n[ordonnee][abscisse] = CAISSE_CIBLE;	
								break;	
							case CAISSE_CIBLE:
   								n[ordonneeInit][abscisseInit] = CIBLE;
								n[ordonnee][abscisse] = CAISSE_CIBLE;		
								break;	
							case JOUEUR_CIBLE:	
   								n[ordonneeInit][abscisseInit] = CIBLE;
								n[ordonnee][abscisse] = JOUEUR_CIBLE;	
								break;	
							default:	return 2;	
}
				case SOL:	switch (n[ordonneeInit][abscisseInit]) {
							case JOUEUR:	
   								n[ordonneeInit][abscisseInit] = SOL;
								n[ordonnee][abscisse] = JOUEUR_CIBLE;	
								break;
							case CAISSE:		
   								n[ordonneeInit][abscisseInit] = SOL;
								n[ordonnee][abscisse] = CAISSE;	
								break;	
							case CAISSE_CIBLE:		
   								n[ordonneeInit][abscisseInit] = CIBLE;
								n[ordonnee][abscisse] = CAISSE;	
								break;	
							case JOUEUR_CIBLE:
   								n[ordonneeInit][abscisseInit] = CIBLE;
								n[ordonnee][abscisse] = JOUEUR;		
								break;	
							default:	return 2;	/*  erreur */
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
void deplacerObjetRetour(Niveau* n, Coup* mouvement, int* nbMvt, int* nbPoussee);

/**
 * \fn void annulerCoup(Coup* mouvement)
 * \brief Annule la derniere action en redeplacant l'objet comme il etait un coup avant
 *
 * \param Mouvement Deplacement souhaite
 */
void annulerCoup(Coup* mouvement);

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
char deplacementPossible(Niveau* n, int ordonnee, int abscisse, int liberte, Direction) {
	char deplacementCaisse = 1;
	switch (Direction) {
				case HAUT:	ordonnee--;
						break;	
				case BAS:	ordonnee++;	
						break;	
				case GAUCHE:	abscisse--;	
						break;	
				case DROITE:	abscisse++;
						break;	
				default:	return 2;	/* direction inconnue */
	}

	switch (n[ordonne][abscisse]) {
				case MUR:	return 1; /* mur : l'objet ne peut pas se deplacer*/
				case CAISSE:	/* caisse */
				case CAISSE_CIBLE:	/* sol */
					if(liberte == 0) {	/* on re-appelle la fonction pour voir si la caisse */
								/* rencontree peut se deplacer */ 
						return deplacementPossible(n, ordonnee, abscisse, 1, Direction);
					}
					if(liberte == 1) {	/* test du deplacement de la caisse */
						return 1;	/* si elle ne peut pas se deplacer */
					}
				default:	return 0;	/* caractere autre */
	}
}


