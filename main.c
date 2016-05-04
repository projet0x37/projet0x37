#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "audio/audioprocess.h"
#include "traitement/traitement.h"
#include "midi/midi.h"


int main(int argc, char** args){
	double * datain;
	Tnote * T;
	int  size;
	double duration;
	double samplerate;
	int minframelength = 4096 ;
	double minframeduration;
	int sizeTmax;


	datain=mainaudio("input.wav",&size,&samplerate);

	duration = (double)size/samplerate;
	minframeduration = (double)4096/samplerate;

	printf("Le fichier audio dure %lf s, en conséquence le tableau Tnote à %d éléments\n",duration,sizeTmax);

	sizeTmax=2*floor(size/4096) + 1;

	T=calloc(sizeTmax,sizeof(*T));
	
	process(T,datain,sizeTmax);

	mainmidi("outputmidi",T,sizeTmax);

	return 0;
}

