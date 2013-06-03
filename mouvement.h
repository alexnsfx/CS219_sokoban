typedef struct
{
	Direction direction;
	int posDepartAbscisse;
	int posDepartOrdonnee;
	char caisseDeplacee; /* 1 si caisse a deplacer, 0 sinon */
} Coup;

typedef enum
{
    HAUT, BAS, GAUCHE, DROITE
} Direction;

void deplacerObjet(Niveau* n, Coup* mouvement , int* nbMvt, int* nbPoussee, int liberte);
void deplacerObjetRetour(Niveau* n, Coup*mouvement, int* nbMvt, int* nbPoussee);

void annulerCoup(Coup* mouvement);

char deplacementPossible(Niveau* n, int ordonnee, int abscisse, int liberte, Direction)

void majNiveau(int ordonneeInit, int abscisseInit, int ordonnee, int abscisse, Niveau* n);
