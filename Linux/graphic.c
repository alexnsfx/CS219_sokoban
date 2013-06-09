#include "graphic.h"

SDLInitError initSDL(SDL_Surface** ecran) {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Erreur d'initialisation de la SDL ! %s\n", SDL_GetError());
		return InitError;
	}
	SDL_WM_SetCaption(NOM_JEU, NULL);

	if(TTF_Init() != 0)
	{
	    fprintf(stderr, "Erreur d'initialisation de SDL_ttf ! %s\n", TTF_GetError());
	    return InitError;
	}

	*ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, COLOR_BIT, SDL_HWSURFACE);
	if(*ecran == NULL) {
		fprintf(stderr, "Impossible de charger le mode video ! %s\n", SDL_GetError());
		return InitError;
	}
	
	SDL_FillRect(*ecran, NULL, SDL_MapRGB((*ecran)->format, 0, 0, 0));
	return InitOk;
}

void freeSDL(SDL_Surface*** sprites) {
	if(sprites != NULL) {
		libereSprites(sprites);
	}

	TTF_Quit();
	SDL_Quit();
}

/**
 * \fn SDLInitError loadSprites(SDL_Surface*** sprites)
 * \brief Charge toutes les sprites dans un tableau.
 * \param SDL_Surface*** Pointeur sur le tableau de pointeurs de SDL_Surface.
 *
 * \return Renvoie InitOk s'il n'y a pas eu de probleme de chargement, InitError sinon.
 */
SDLInitError loadSprites(SDL_Surface*** sprites) {
	int i;
	if(sprites == NULL || *sprites == NULL) {
		return InitError;
	}

	/* definition des images a afficher */
	(*sprites)[S_HAUT] = SDL_LoadBMP("sprites/HAUT.bmp");
	(*sprites)[S_BAS] = SDL_LoadBMP("sprites/BAS.bmp");
	(*sprites)[S_GAUCHE] = SDL_LoadBMP("sprites/GAUCHE.bmp");
	(*sprites)[S_DROITE] = SDL_LoadBMP("sprites/DROITE.bmp");
	(*sprites)[S_JOUEUR_CIBLE] = SDL_LoadBMP("sprites/JOUEUR_CIBLE.bmp");
	(*sprites)[S_MUR] = SDL_LoadBMP("sprites/MUR.bmp"); 
	(*sprites)[S_CIBLE] = SDL_LoadBMP("sprites/CIBLE.bmp"); 
	(*sprites)[S_CAISSE] = SDL_LoadBMP("sprites/CAISSE.bmp"); 
	(*sprites)[S_CAISSE_CIBLE] = SDL_LoadBMP("sprites/CAISSE_CIBLE.bmp"); 
	(*sprites)[S_SOL] = SDL_LoadBMP("sprites/SOL.bmp"); 

	for(i = 0; i < NB_SPRITE; i++) {
		if((*sprites)[i] == NULL) {
			fprintf(stderr, "La ligne %d du tableau de sprites est nul.\n", i+1);
			return InitError;
		}
	}

	return InitOk;	
}

void libereSprites(SDL_Surface*** sprites) {
	int i;	
	if(sprites == NULL || *sprites == NULL) {
		return;
	}

	for(i = 0; i < NB_SPRITE; i++) {
		if((*sprites)[i] != NULL) {
			SDL_FreeSurface((*sprites)[i]);
		}
	}
	free(*sprites);
}

void dessineEntete(SDL_Surface* fond) {
	SDL_Surface *entete, *texte;
	SDL_Color couleurNoire = {0, 0, 0};
	TTF_Font* police = NULL;
	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;

	if(fond == NULL) {
		return;
	}

	police = TTF_OpenFont(NOM_POLICE, 55);
	if(!police) {
		fprintf(stderr, "Erreur d'ouverture de la police dans dessineEntete : %s\n", TTF_GetError());
		return;
	}

	entete = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_FENETRE, ENTETE_HAUTEUR * TAILLE_BLOC, COLOR_BIT, 0, 0, 0, 0); 
	if(!entete) {
		fprintf(stderr, "Erreur lors du dessin du bandeau d'entete dans dessineEntete : %s\n", SDL_GetError());
		TTF_CloseFont(police);
		return;
	}

	SDL_FillRect(entete, NULL, SDL_MapRGB(fond->format, 255, 255, 255));

	/* Titre */
	texte = TTF_RenderText_Blended(police, TITRE_ENTETE, couleurNoire);
	if(!texte) {
		fprintf(stderr, "Erreur de rendu du titre dans dessineEntete : %s\n", TTF_GetError());
		SDL_FreeSurface(entete);
		TTF_CloseFont(police);
		return;
	}

	SDL_BlitSurface(texte, NULL, entete, &pos);
	SDL_FreeSurface(texte);

	SDL_BlitSurface(entete, NULL, fond, &pos);
	SDL_FreeSurface(entete);

	TTF_CloseFont(police);
}

void dessineCompteurs(SDL_Surface* fond, unsigned int nbMvt, unsigned int nbPoussee) {
	char chaineCompteur[16] = {0};
	SDL_Surface *texte;
	SDL_Color couleurNoire = {0, 0, 0};
	TTF_Font* policeCompteur = NULL;
	SDL_Rect pos;

	if(fond == NULL) {
		return;
	}

	policeCompteur = TTF_OpenFont(NOM_POLICE, 30);
	if(!policeCompteur) {
		fprintf(stderr, "Erreur d'ouverture de la police dans dessineCompteurs : %s\n", TTF_GetError());
		return;
	}

	/* Compteur mouvement */
	sprintf(chaineCompteur, "Mouvements : %d", nbMvt);
	pos.x = TAILLE_BLOC * 17;
	pos.y = 0;

	texte = TTF_RenderText_Blended(policeCompteur, chaineCompteur, couleurNoire);
	if(!texte) {
		fprintf(stderr, "Erreur de rendu du compteur mouvement dans dessineCompteurs : %s\n", TTF_GetError());
		TTF_CloseFont(policeCompteur);
		return;
	}

	SDL_BlitSurface(texte, NULL, fond, &pos);
	SDL_FreeSurface(texte);

	/* Compteur poussee */
	pos.x = TAILLE_BLOC * 17;
	pos.y = TAILLE_BLOC;
	sprintf(chaineCompteur, "Poussees : %d", nbPoussee);

	texte = TTF_RenderText_Blended(policeCompteur, chaineCompteur, couleurNoire);
	if(!texte) {
		fprintf(stderr, "Erreur de rendu du compteur poussee dans dessineCompteurs : %s\n", TTF_GetError());
		TTF_CloseFont(policeCompteur);
		return;
	}

	SDL_BlitSurface(texte, NULL, fond, &pos);
	SDL_FreeSurface(texte);

	TTF_CloseFont(policeCompteur);
}

void dessinerDescriptif(SDL_Surface* fond, SpriteType objetADessiner, SDL_Surface*** sprites) {
	SDL_Surface *titre;
	SDL_Color couleurNoire = {0, 0, 0};
	SDL_Rect posDroit;
	SDL_Rect posGauche;
	SDL_Rect posImD;
	SDL_Rect posImG;
	TTF_Font* police = NULL;
	
	if(fond == NULL || sprites == NULL || *sprites == NULL) {
		return;
	}

	posDroit.x = 400;
	posDroit.y = 20;
	posGauche.x = 700;
	posGauche.y = 20;
	posImD.x = 576;
	posImD.y = 20;
	posImG.x = 896;
	posImG.y = 20;

	police = TTF_OpenFont(NOM_POLICE, 25);
	if(!police) {
		fprintf(stderr, "Erreur d'ouverture de la police dans dessinerDescriptif : %s\n", TTF_GetError());
		return;
	}

	titre = TTF_RenderText_Blended(police, "Clic droit", couleurNoire);
	if(!titre) {
		fprintf(stderr, "Erreur de rendu dans dessinerDescriptif : %s\n", TTF_GetError());
		TTF_CloseFont(police);
		return;
	}

	SDL_BlitSurface(titre, NULL, fond, &posDroit);
	SDL_FreeSurface(titre);

	titre = TTF_RenderText_Blended(police, "Clic gauche", couleurNoire);
	if(!titre) {
		fprintf(stderr, "Erreur de rendu dans dessinerDescriptif : %s\n", TTF_GetError());
		TTF_CloseFont(police);
		return;
	}

	SDL_BlitSurface(titre, NULL, fond, &posGauche);
	SDL_FreeSurface(titre);

	SDL_BlitSurface((*sprites)[S_SOL], NULL, fond, &posImD);
	SDL_BlitSurface((*sprites)[objetADessiner], NULL, fond, &posImG);

	TTF_CloseFont(police);
}

void dessinerElement(SpriteType objetADessiner, int abscisse, int ordonnee, SDL_Surface* fond, SDL_Surface*** sprites, Niveau* level, int w, int h) {
	SDL_Rect pos;
	pos.x = (int)(abscisse/TAILLE_BLOC);
	pos.y = (int)(ordonnee/TAILLE_BLOC);
	
	if(fond == NULL || sprites == NULL || *sprites == NULL || level == NULL) {
		return;
	}

	if ( pos.y>1 && pos.y<(h+2) && pos.x>=(int)((NB_BLOCS_LARGEUR - w)/2) && pos.x<( w + (int)((NB_BLOCS_LARGEUR - w)/2) )  ){

		(*level)[pos.y-2][pos.x-(int)((NB_BLOCS_LARGEUR - w)/2)] = choixCaractere(objetADessiner);

		pos.x *= TAILLE_BLOC;
		pos.y *= TAILLE_BLOC;
		SDL_BlitSurface((*sprites)[objetADessiner], NULL, fond, &pos);
	}
}

void dessineNiveau(Niveau* n, SDL_Surface* fond, unsigned int largeur, unsigned int hauteur, SDL_Surface*** sprites) {
	SDL_Rect pos;
	SpriteType numeroSprite;
	int i, j;

	if(sprites == NULL || *sprites == NULL || fond == NULL || n == NULL || *n == NULL) {
		return;
	}

	for(j = 0; j < hauteur; j++) {
		for(i = 0; i < largeur; i++) {
			pos.x = ((int)((NB_BLOCS_LARGEUR - largeur)/2) + i) * TAILLE_BLOC;
			pos.y = (ENTETE_HAUTEUR + j) * TAILLE_BLOC;
			numeroSprite = choixSprite((*n)[j][i]);
			SDL_BlitSurface((*sprites)[numeroSprite], NULL, fond, &pos);
		}
	}
}

void effacerNiveau(SDL_Surface* fond) {
	if(fond != NULL) {
		SDL_FillRect(fond, NULL, SDL_MapRGB(fond->format, 0, 0, 0));
	}
}

SpriteType choixSprite(char objet) {
	switch(objet) {
		case JOUEUR:
			return S_BAS;
		case MUR:
			return S_MUR;
		case CIBLE:
			return S_CIBLE;
		case CAISSE:
			return S_CAISSE;
		case SOL:
			return S_SOL;
		case CAISSE_CIBLE:
			return S_CAISSE_CIBLE;
		case JOUEUR_CIBLE:
			return S_JOUEUR_CIBLE;
		default:
			return S_SOL;
	}
}

char choixCaractere(SpriteType sprite) {
	switch(sprite) {
		case S_BAS:
			return JOUEUR;
		case S_MUR:
			return MUR;
		case S_CIBLE:
			return CIBLE;
		case S_CAISSE:
			return CAISSE;
		case S_SOL:
			return SOL;
		case S_CAISSE_CIBLE:
			return CAISSE_CIBLE;
		case S_JOUEUR_CIBLE:
			return JOUEUR_CIBLE;
		default:
			return SOL;
	}
}
