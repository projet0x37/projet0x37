#include<stdlib.h>
#include<stdio.h>	
#include "midi.h"

int est_vide(liste L){
	return !L;}

liste ajouttete3(liste l, double duree, char note){
	liste c = calloc(1, sizeof(*c));
	if(c == NULL) return NULL;
	c->duree = duree;
	c->note = note;
	c->suiv = l;
	return c;
}

liste supprimer_tete(liste l){
	if( !est_vide(l)){
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
	if(!l->suiv || n==1 ) return supprimer_tete(l);
	if(n==1) return supprimer_tete(l);
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


liste Tri(liste *l){// on donne un pointeur pour pouvoir supprimer de la mémoire la liste *l 
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
		if(p){
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

main(){
	//int i;
	liste l=NULL;
	liste p;
	l=ajouttete3(l,(double)(1),63);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(3),62);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(2),61);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(24),46);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(0),78);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(3),61);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(3),61);
	printf("%f,%d\n",l->duree,l->note);
	l=ajouttete3(l,(double)(3),61);
	printf("%f,%d\n",l->duree,l->note);
	l=Tri(&l);
	for(p=l;p;p=p->suiv){
		printf("t %f,%d\n",p->duree,p->note);
		}
	}
		
		
