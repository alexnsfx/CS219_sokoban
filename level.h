/**
 * \file level.h
 * \brief (Prototypes) Fonctions utiles a la lecture d'un niveau
 * \author Dorialex
 * \version 0.1
 * \date 16 mai 2013
 *
 * Prototypes des fonctions du fichier level.c, regroupant toutes les fonctions utiles a la lecture d'un niveau.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <stdio.h>

/**
 * \fn char getTailleNiveau(FILE* niveaux, int* largeur, int* hauteur)
 * \brief Donne la largeur et la hauteur du niveau desire
 *
 * \param niveaux Pointeur sur le fichier de niveaux
 * \param largeur Pointeur sur l'entier qui contiendra la largeur du niveau
 * \param hauteur Pointeur sur l'entier qui contiendra la hauteur du niveau
 * \return Renvoie un code d'erreur, 0 si tout s'est bien passe
 */
char getTailleNiveau(FILE* niveaux, int* largeur, int* hauteur);

char** readLevel(char* path, int numero);

#endif
