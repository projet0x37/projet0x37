#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "audio/audioprocess.h"
#include "traitement/traitement.h"
#include "midi/midi.h"


int main(int argc, char** args){
	double * datain;
	Tnote T;
	int  size;
	double duration;
	double samplerate;
	int minframelength = 4096 ;
	double minframeduration;
	int sizeTmax;
	double timeresolutionsms;


	datain=mainaudio("input.wav",&size,&samplerate);

	duration = (double)size/samplerate;
	minframeduration = (double)4096/samplerate;

	printf("Le fichier audio dure %lf s, en conséquence le tableau Tnote à %d éléments\n",duration,sizeTmax);

	sizeTmax=2*floor(size/4096) + 2; // on prend une taille suffisamment large

	T=calloc( sizeTmax , sizeof(*T) );

	initTnote( T , sizeTmax );

	mainprocessing( T , datain , sizeTmax );
	
	printf("Entrez une résolution temporelle minimum pour la partition, sachant que la durée de la porte la plus petite utilisée est %lf\n",

	T=resizeTnote( T , & sizeTmax , timeresolutionms );

	mainmidi("outputmidi",T,sizeTmax);

	return 0;
}

