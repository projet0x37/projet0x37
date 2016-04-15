#include <stdio.h>
#include <stdlib.h>
#include "textexport.h"
#include "audioprocess.h"

int main(){
	double * data;
	int taille;
	int taille_utile;
	data=mainaudio("test.wav",&taille);
	printf("Il y a %d données importées, combien voulez vous en importer ?\n",taille);
	scanf("%d",&taille_utile);	
	textexport("test.txt",data,taille,taille_utile);
	return 0;
}
