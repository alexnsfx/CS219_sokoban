/**
 * \file testunitaire.c
 * \brief Test unitaire de la fonction calculPosition
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#include <stdlib.h>
#include "mouvement.h"

int main(int argc, char* argv[]) {
	char err = 0;
	
	Position p;
	p.x = 0;
	p.y = 0;

	calculPosition(&p, HAUT, Positif);
	err += (p.x != 0 || p.y != -1);

	calculPosition(&p, HAUT, Negatif);
	err += (p.x != 0 || p.y != 0);

	calculPosition(&p, GAUCHE, Positif);
	err += (p.x != -1 || p.y != 0);

	calculPosition(&p, GAUCHE, Negatif);
	err += (p.x != 0 || p.y != 0);

	calculPosition(&p, DROITE, Positif);
	err += (p.x != 1 || p.y != 0);

	calculPosition(&p, DROITE, Negatif);
	err += (p.x != 0 || p.y != 0);

	calculPosition(&p, BAS, Positif);
	err += (p.x != 0 || p.y != 1);

	calculPosition(&p, BAS, Negatif);
	err += (p.x != 0 || p.y != 0);

	if(err) {
		printf("Test unitaire echoue avec %d erreur(s) ! La fonction calculPosition est invalide.\n", err);
	}
	else {
		printf("Test unitaire execute avec succes ! La fonction calculPosition est valide.\n");
	}

	return err;
}
