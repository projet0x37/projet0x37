#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "audio/audioprocess.h"
#include "traitement/fonctions.h"
#include "midi/midi.h"
#include "textexport/textexport.h"
#include <fftw3.h>

int main(int argc, char** args){
	double * datain;
	Tnote T;
	int  size;
	double duration;
	double samplerate;
	int frames = 8192 ;
	double minframeduration;
	int sizeTmax;
	double timeresolutionsms;
	fftw_complex * out;
	fftw_plan p;
	double * DSP;
	int i;
	double * in;
	int sizeout;
	int position;
	double t0;
	double tf;

	datain=mainaudio("input.wav",&size,&samplerate);

	in = fftw_malloc( sizeof(double)*frames);
	sizeout = frames/2 + 1;
	out = fftw_malloc(sizeof(fftw_complex)*sizeout);
	p = fftw_plan_dft_r2c_1d( frames , in , out , FFTW_ESTIMATE );

	DSP=calloc(sizeout,sizeof(double));

	duration = (double)size/samplerate;
	minframeduration = (double)frames/samplerate;
	sizeTmax = 2*floor(size/4096) + 2; // on prend une taille suffisamment large
	
	printf("Le fichier audio dure %lf s , il comporte %d éléments, la fréquence d'échantillonage est de %lf, en conséquence le tableau Tnote à %d éléments\n",duration,size,samplerate,sizeTmax);
	
	printf("Entrer le position du début de la porte ( sachant qu'on se décale d'une demie porte )\n");
	scanf("%d",&position);

	for(i=0 ; i < frames ; i++){
		if (i + position*frames/2 < size)in[i] = datain[ i + position*frames/2 ];
		else in[i] = 0;
	}
	t0 = position*frames/(2*samplerate);
	tf = t0 + frames/samplerate;
	printf("T0 %lf , Tf %lf \n",t0,tf);
	fftw_execute(p);

	for(i=0;i<sizeout;i++) DSP[i] = ( out[i][0]*out[i][0] + out[i][1]*out[i][1] );

	textexport("datainoutput",in,frames,frames);
	textexport("DSPout",DSP,sizeout,sizeout);

	fftw_free(out);
	fftw_destroy_plan(p);

	/*
	T=calloc( sizeTmax , sizeof(*T) );

	initTnote( T , sizeTmax );

	mainprocessing( T , datain , sizeTmax );
	
	printf("Entrez une résolution temporelle minimum pour la partition, sachant que la durée de la porte la plus petite utilisée est %lf\n",

	T=resizeTnote( T , & sizeTmax , timeresolutionms );

	mainmidi("outputmidi",T,sizeTmax);
	*/
	return 0;
}

