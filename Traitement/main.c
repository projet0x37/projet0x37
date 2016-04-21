//
//  main.c
//  Projet Info
//
//  Created by Vincent Maladiere on 13/04/16.
//  Copyright © 2016 Vincent Maladiere. All rights reserved.
//

#include <stdio.h>
#include "liste.h"
#include "graphe.h"




int main() {
    graphe G = nouveau_graphe(3,4);
    affiche_graphe(G,3);
    puts("DAYUM NIGGA \n");
    detruit_graphe(G,3);
    //affiche_graphe(G,3);
    return 0;
}


/*
FILE* f = fopen("nomfichier","r+w");
int s, compteur;
scanf("%d",&s); //Entrer le point de départ

graphe g = creer_graphe();
g->arc = calloc(1,sizeof(g->arc));
g->arc->val = 0;
for(i=1;i<nX-1;i++){
    g->arc = ajout_queue(F_MAX, g->arc); //initialisation des nX sommets
    g->arc = g->arc->suiv;
}

//On crée un tableau de liste comme sur l'énoncé


fseek(f,0,SEEK_SET); //on se place au début du fichier
while (fgetc(fp) != '\n'); //on saute la première ligne
while(compteur<=nX){    //lecture des X lignes du fichier
    fscanf(f,"%d %lf %lf %s", &(numero), &(lat), &(longi), line);
    
    compteur++;

*/