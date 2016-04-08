#include <stdio.h>
#include <stdlib.h>
#include "midi.h"
#include "listemidi.h"

//bufferc
bufferc ajout_tetebuff(char r,bufferc l){
	bufferc p=NULL;
	p=calloc(1,sizeof(*p));
	(p->r)=r;
	p->suiv=l;
	return p;
	}

bufferc supprimer_tetebuff(bufferc l){
	bufferc p;
	if(l){
		p = l->suiv;
		free(l);
		return p;}
	else return NULL;
}

void freebuff(bufferc l){
	bufferc p = l;
	while(p) p=supprimer_tetebuff(p);
}

//liste
void freeliste(liste l){
	liste p = l;
	while(p) p=supprimer_teteliste(p);
	}

liste ajouttete3(liste l, double duree, char note){
	liste c = calloc(1, sizeof(*c));
	if(c == NULL) return NULL;
	c->duree = duree;
	c->note = note;
	c->suiv = l;
	return c;
}

liste supprimer_teteliste(liste l){
	if(l){
		liste p;
		p = l->suiv;
		free(l);
		return p;}
	else return NULL;
}

liste supprimen(int n,liste l){
	int i;
	liste p=l;
	liste a=NULL;
	if(!l) return l;
	if(!l->suiv || n==1 ) return supprimer_teteliste(l);
	if(n==1) return supprimer_teteliste(l);
	for(i=0; i<n-2 && (p->suiv)->suiv ; i++, p=p->suiv);

	a=p->suiv;

	if(!((p->suiv)->suiv)){ 
		free(p->suiv);
		p->suiv=NULL;
		return l;
		}
	p->suiv=(p->suiv)->suiv;
	free(a);
	return l;
}


liste tri(liste *l){// on donne un pointeur pour pouvoir supprimer de la mémoire la liste *l 
	double dureemax;
	liste p = *l;
	liste r = NULL;
	int i = 1;
	int imax = 1;
	dureemax = p->duree;
	char notedureemax = p->note;
	while( *l ){ // tant que tout n'est pas trié ( *l non vide ) on continue
		while(p){ // Boucle pour chercher le maximum et sa position dans *l (isupp)
			if( p->duree > dureemax){ // on en trouve un et on garde en mémoire ses caractéristiques
				dureemax=p->duree;
				notedureemax=p->note;
				imax=i;
				i=i+1; // on incrémente pour regarder au rang suivant
				p=p->suiv;	
				}
			else{
				i=i+1; // on en trouve pas on incrémente quand même
				p=p->suiv;
				}
	
		} // sortie de boucle , on a trouvé le maximum
		r=ajouttete3(r, dureemax, notedureemax); // on ajoute le maximum trouvé dans r
		*l=supprimen(imax,*l); // on le supprime de *l
		p=*l; // on réinitialise p  et les variables, on recommence le tri
		if(p){ // pour eviter un segmentation fault ( evite de regarder NULL->duree qui n existe pas ) lorsque tout est trié
			dureemax=p->duree;
			notedureemax=p->note;
			imax=1;}
		i=1;
		
	}
	return r; // on retourne la liste triée
}
/*On place en tête le maillon de plus grande duree avant de le supprimer du reste de 

la liste, on considere alors la liste sans cet élement et on poursuit jusqu'a ce que tout 

soit trié*/


liste creationLi(liste l){
	liste p=NULL;
	if(!l) return NULL;
	p=calloc(1,sizeof(*p));
	p->note=l->note;
	p->duree=p->duree;
	p->suiv=creationLi(l->suiv);
	return p;
}


liste tabtoliste(chord tab[]){			//OK
	liste l=NULL;
	liste p=NULL;
	int i=1;
	l=calloc(1,sizeof(*p));
	p=l;
	for(i=0; i<31 && tab[i].note > -1 ; i++){
		l->note=tab[i].note;
		l->duree=tab[i].duree;
		if(tab[i+1].note < 0) l->suiv=NULL;
		else{
			l->suiv=calloc(1,sizeof(*l));
			l=l->suiv;
		}
	}
	return(p);
}



liste concat(liste l1, liste l2){
	liste p=l1;
	if(!l1) return l2;
	while(p->suiv) p=p->suiv;
	p->suiv=l2;
	return l1;
}
 
