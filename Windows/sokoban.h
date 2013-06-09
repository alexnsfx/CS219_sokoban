/**
 * \file sokoban.h
 * \brief Fichier de configuration
 * \author Doriane PERARD et Alex NODET
 * \version 0.1
 * \date 7 juin 2013
 *
 */

#ifndef SOKOBAN_H
#define SOKOBAN_H

#define NOM_JEU "Pokeban - Le sokoban pokemon !"
#define TITRE_ENTETE "Pokeban"
#define FICHIER_NIVEAUX "levels.lvl"
#define FICHIER_TEMP "levels.tmp"
#define NOM_POLICE "pokemon.ttf"

#define MAX_LEVEL 88
#define BUFFER_WIDTH 64
#define SAVE_BUFFER_WIDTH 45

#define COLOR_BIT 32
#define TAILLE_BLOC 32
#define NB_BLOCS_LARGEUR 30
#define NB_BLOCS_HAUTEUR 18
#define ENTETE_HAUTEUR 2
#define LARGEUR_FENETRE TAILLE_BLOC * NB_BLOCS_LARGEUR
#define HAUTEUR_FENETRE TAILLE_BLOC * (NB_BLOCS_HAUTEUR + ENTETE_HAUTEUR)

#define NB_SPRITE 10
#define	JOUEUR '@'
#define	MUR '#'
#define	CIBLE '.'
#define	CAISSE '$'
#define	SOL ' '
#define	CAISSE_CIBLE '*'
#define	JOUEUR_CIBLE '+'

typedef enum {
	Ok, 
	AllocError, 
	UndefinedPile
} PileError;

typedef char** Niveau;

/**
 * \struct Position
 * \brief Definit une position.
 */
typedef struct {
	int x;	/*!< Abscisse. */
	int y;	/*!< Ordonnee. */
} Position;

#endif
