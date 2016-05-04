//
//  main.c
//  A man named Klapury
//
//  Created by Vincent Maladiere on 08/04/16.
//  Copyright © 2016 Vincent Maladiere. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fonctions.h"
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include "affiche.h"
#undef main

int main(){
    double * X = calloc(6000,sizeof(X));
    double * Y = calloc(6000,sizeof(Y));
    double * MA = calloc(6000,sizeof(MA));
    double * Z = calloc(6000,sizeof(Z));
    double largeur = 50;
    int taille = 6000;
    
    int i;
    for(i=0;i<50;i++){
        *(X+i) = 0;
    }
    for(i=50;i<6000;i++){
        *(X+i) = frand_a_b(0,50);
    }
   
    *(X+396) = 150*2;
    *(X+397) = 250*2;
    *(X+398) = 350*2;
    *(X+399) = 390*2;
    *(X+400) = 400*2;
    *(X+401) = 390*2;
    *(X+402) = 350*2;
    *(X+403) = 250*2;
    *(X+404) = 150*2;
    
    *(X+410) = 76*2;
    *(X+411) = 103*2;
    *(X+412) = 108*2;
    *(X+413) = 169*2;
    *(X+414) = 290*2;
    *(X+415) = 288*2;
    *(X+416) = 287*2;
    *(X+417) = 280*2;
    *(X+418) = 279*2;
    *(X+419) = 278*2;
    *(X+420) = 270*2;
    *(X+421) = 260*2;
    *(X+422) = 258*2;
    *(X+423) = 250*2;
    *(X+424) = 240*2;
    *(X+425) = 210*2;
    *(X+426) = 135*2;

    Y = Y_extraction(X,taille);
    MA = moving_average(Y,largeur,taille);
    Z = Z_calc(Y,MA,taille);
    
    //Pour connaître les valeurs de votre choix :
    //int a;
    //puts("Entrez oune entier");
    //scanf("%d",&a);
    //printf("%lf\n",Y[a]);
    /*
    printf("MA = %lf\n",MA[a]);
    printf("Z = %lf\n",Z[a]);*/
    
    SDL_Surface * ecran = NULL;
    
    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL
    
    ecran = SDL_SetVideoMode(1000, 800, 32, SDL_HWSURFACE); // Ouverture de la fenêtre
    
    SDL_WM_SetCaption("Spectre de X", NULL);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,255,255,255));
    drawLine(ecran,0,0,1000,0,SDL_MapRGB(ecran->format,210,130,20));
    
    for(i=0;i<1000;i++){
        drawLine(ecran,i,-X[i],i+1,-X[i+1],SDL_MapRGB(ecran->format,255,0,0));
        drawLine(ecran,i,-Y[i],i+1,-Y[i+1],SDL_MapRGB(ecran->format,0,255,0));
        drawLine(ecran,i,-MA[i],i+1,-MA[i+1],SDL_MapRGB(ecran->format,130,130,50));
        drawLine(ecran,i,-Z[i],i+1,-Z[i+1],SDL_MapRGB(ecran->format,0,0,255));
        if(i%100==0){
                drawLine(ecran,i,20,i,-20,SDL_MapRGB(ecran->format,210,130,20));
        }
    }
    SDL_Flip(ecran); /* Mise à jour de l'écran */
    pauuse(); // Mise en pause du programme
    SDL_Quit(); // Arrêt de la SDL
    
    return EXIT_SUCCESS; // Fermeture du programme
    
    
    
    return 0;
}

