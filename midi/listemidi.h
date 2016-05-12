#ifndef LISTEMIDI_H
#define LISTEMIDI_H

#include <stdlib.h>
#include <stdio.h>
#include "midi.h"

typedef struct{
	char note;
	double duree;
	int kech;
	} chord;

struct maillon {
	char note;
	double duree;
	struct maillon* suiv;
	};

typedef struct maillon* liste;


struct reste{
	char r;
	struct reste* suiv;
	};

typedef struct reste* bufferc;


// pour les liste de délais
bufferc ajout_tetebuff(char r, bufferc buf);
bufferc supprimer_tetebuff(bufferc l);
void freebuff(bufferc * l);

// pour les listes de notes
//int est_vide(liste L); //Unused
//liste triLi(liste Li); //Unused
void freeliste(liste * l);
liste ajouttete3(liste l, double duree, char note);
liste supprimer_teteliste(liste l);
liste supprimen(int n,liste l);
liste tri(liste *l);
liste creationLi(liste l);
liste tabtoliste(chord tab[]);
liste concat(liste l1, liste l2);

#endif
