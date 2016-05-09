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
	int  size;
	double duration;
	double samplerate;
	int sizeframe = 8192 ;
	int sizeTmax;
	int i;
	int k0,k1;
	double * in;
	int sizeout;
	int position;
	double t0;
	double tf;
	int b;
	frame DSP;
	Tnote T;
	frame Z;
	frame N;
	double * B_m0_m2 = calloc(BNMAX*2,sizeof(double));
	double ** Mat;
	datain=mainaudio("input.wav",&size,&samplerate);
	duration=(double)size/samplerate;
	
	printf("Le fichier audio dure %lf s , il comporte %d éléments, la fréquence d'échantillonage est de %lf, quelle taille de porte voulez vous choisir ?\n",duration,size,samplerate);
	printf(" 1 : 2**11 soit %lf s\n",(double)2048/samplerate);
	printf(" 2 : 2**12 soit %lf s\n",(double)4096/samplerate);
	printf(" 3 : 2**13 soit %lf s\n",(double)8192/samplerate);
	printf(" 4 : 2**14 soit %lf s\n",(double)16384/samplerate);
	scanf("%d",&b);
	switch (b)
	{
	case 1 : 
		sizeframe = 2048;
		break;
	case 2 : 
		sizeframe = 4096;
		break;
	case 3 : 
		sizeframe = 8192;
		break;
	case 4 : 
		sizeframe = 16384;
		break;
	default :
		sizeframe = 4096;
		break;
	}
	printf("Vous avez choisi une largeur de %lf s , sachant que lors du traitement on se déplace d'une demie-porte\n", (double)sizeframe/samplerate);
	printf("décalage porte ?\n");
	scanf("%d",&position);
	sizeout = sizeframe/2 +1;
	in = fftw_malloc(sizeframe*sizeof(double));
	for(i=0 ; i < sizeframe ; i++){
		if (i + position*sizeframe/2 < size)in[i] = datain[ i + position*sizeframe/2 ];
		else in[i] = 0;
	}
	DSP=calloc(sizeout,sizeof(double));
	short_time_DSP( in , sizeframe, DSP);
	sizeTmax=2*(size/sizeframe)+2;
	T=calloc(sizeTmax,sizeof(*T));
	
	Mat=MatrixBW( sizeout , round(FMIN*sizeframe/samplerate) , round(BMIN*sizeframe/samplerate) ,B_m0_m2);
	t0 = position*sizeframe/samplerate;
	tf = t0 + sizeframe/samplerate;
	printf("T0 %lf , Tf %lf \n",t0,tf);
	printf("%d\n",BNMAX);
	textexport_Mat("Matout",Mat,sizeout,BNMAX);
	//textexport("Zout",Z,sizeout,sizeout);
	//textexport("DSPout",DSP,sizeout,sizeout);
	//textexport("Yout",Y,sizeout,sizeout);
	//textexport("Nout",N,sizeout,sizeout);
	
	return 0;
}

