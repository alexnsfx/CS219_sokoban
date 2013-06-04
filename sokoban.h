#ifndef SOKOBAN_H
#define SOKOBAN_H

#define NOM_JEU "Pokeban"
#define BUFFER_WIDTH 64

#define COLOR_BIT 32
#define TAILLE_BLOC 32
#define NB_BLOCS_LARGEUR 30
#define NB_BLOCS_HAUTEUR 18
#define LARGEUR_FENETRE TAILLE_BLOC*NB_BLOCS_LARGEUR
#define HAUTEUR_FENETRE TAILLE_BLOC*NB_BLOCS_HAUTEUR

#define NB_SPRITE 11

#define	JOUEUR '@'
#define	MUR '#'
#define	CIBLE '.'
#define	CAISSE '$'
#define	SOL ' '
#define	CAISSE_CIBLE '*'
#define	JOUEUR_CIBLE '+'

typedef char** Niveau;

typedef struct {
	int x;
	int y;
} Position;

#endif
