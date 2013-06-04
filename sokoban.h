#ifndef SOKOBAN_H
#define SOKOBAN_H

#define TAILLE_BLOC 32
#define NB_BLOCS_LARGEUR 30
#define NB_BLOCS_HAUTEUR 18
#define LARGEUR_FENETRE TAILLE_BLOC*LARGEUR
#define HAUTEUR_FENETRE TAILLE_BLOC*HAUTEUR

#define	JOUEUR '@'
#define	MUR '#'
#define	CIBLE '.'
#define	CAISSE '$'
#define	SOL ' '
#define	CAISSE_CIBLE '*'
#define	JOUEUR_CIBLE '+'

typedef char** Niveau;

#endif
