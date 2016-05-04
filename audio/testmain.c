#include <stdio.h>
#include <stdlib.h>
#include "textexport.h"
#include "audioprocess.h"

int main(){
	double * data;
	int taille;
	int taille_utile;
	double samplerate;
	if((data = mainaudio("test.wav", &taille,&samplerate))==NULL){
	  return EXIT_FAILURE;
	}
	printf("La fréquence d'échantillonage est de %lf Hz\n",samplerate);
	printf("Il y a %d données importées, combien voulez vous en importer ?\n",taille);
	scanf("%d",&taille_utile);	
	textexport("test.txt",data,taille,taille_utile);
	return 0;
}
