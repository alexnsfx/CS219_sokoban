#ifndef SOKOBAN_H
#define SOKOBAN_H

#define TAILLE_BLOC 32
#define NB_BLOCS_LARGEUR 30
#define NB_BLOCS_HAUTEUR 18
#define LARGEUR_FENETRE TAILLE_BLOC*LARGEUR
#define HAUTEUR_FENETRE TAILLE_BLOC*HAUTEUR

typedef enum {
	Joueur = '@',
	Mur = '#',
	Cible = '.',
	Caisse = '$',
	Sol = ' ',
	CaisseCible = '*',
	JoueurCible = '+'
} MapObject;

typedef char** NIVEAU;

#endif
