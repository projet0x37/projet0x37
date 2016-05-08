#ifndef fonctions_h
#define fonctions_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../midi/midi.h"

typedef double * frame;
double * creertab( int n);
double incertitude ( int n );
char correspondancenote( float fech , int n );
double * Y_extraction( double * X , int taille, int k0 , int k1 );
void moving_average( double * Y , double ratio , int taille , frame movingA );
double * Z_calc( double * Y, double * N , int taille );
double * noisesup( frame X , int k0 , int k1 , int taille , double ratio , frame N );
int processing_init( frame X , frame Lmax , frame Npow , int taille , double threshold , int k0 , int k1 );
int iteration_checking(frame X, frame Lmaxi,frame Npow, int taille,double treshold, int k0, int k1);
double SNR_calc( frame X , frame N , int sizeframe);
int F0fromL(double* L, int taille);
double mean( double* T , double m0 , double m2 );
int round( double value );
double* zeros( int l );
double* functionBW ( double Bmin , int N , double fs , int l , double kb , double ratio , double* M0 , double* M2 );
double* Z_smoothing(double* z, int taille,double fs,int N,double ratio,int k);
double * npow( double* moyennex, int k1,int k0,int taille);
void initTnote( Tnote T , int sizeTmax );
frame short_time_DSP( frame x , int sizeframe );
int boucle(chord * tabchord , frame Z , int sizeframe , double SNR , int nmax , int kmin , double thresv0 , double thresvi , int k0 , int k1)
int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, int kmin , int k1 , int k0);
void mainprocessing( Tnote  T , double * datain , int sizeTmax , double samplerate , int sizeframe);
Tnote  resizeTnote( Tnote  T , int sizeTmax , double timeresolution );
double max_valueandposition_frame(frame X , int sizeframe , int * k);
frame arraymultiplication( frame X, frame Y, int sizeframe);

#endif /* fonctions_h */
