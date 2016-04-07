#ifndef LISTEMIDI_H
#define LISTEMIDI_H

#include <stdlib.h>
#include <stdio.h>
#include "midi.h"


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



// pour les listes de notes
int est_vide(liste L);
liste ajouttete3(liste l, double duree, char note);
liste triLi(liste Li);
liste supprimer_teteliste(liste l);
liste supprimen(int n,liste l);
liste tri(liste *l);
void freeliste(liste l);
liste creationLi(liste l);
liste concat(liste l1, liste l2);
// pour les liste de délais
bufferc ajouttete(bufferc buf);
bufferc supprimer_tetebuff(bufferc l);
void freebuff(bufferc l);
#endif
