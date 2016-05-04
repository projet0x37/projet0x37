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
double * Y_extraction( double * X , int taille, int k0 , k1 );
double * moving_average( double * Y , double largeur , int taille);
double * Z_calc( double * Y, double * N , int taille );
void fosup( frame Zsmoothed , frame Z , int taille );
int processing_init( frame X , frame Lmax , frame Npow , int taille , double threshold , int k0 , int k1 );
int iteration_checking(frame X, frame Lmaxi,frame Npow, int taille,double treshold, int k0, int k1);
int F0fromL(double* L, int taille);
double mean( double* T , double m0 , double m2 );
double round( double value );
double* zeros( int l );
double* functionBW ( double Bmin , int N , double fs , int l , double kb , double ratio , double* M0 , double* M2 );
void initTnote( Tnote T , int sizeTmax );
int frameprocessing( tabchord C , double * frame , int sizeframe , double time);
void mainprocessing( Tnote  T , double * datain , int sizeTmax , double samplerate);
Tnote  resizeTnote( Tnote  T , int sizeTmax , double timeresolution );
double* Z_smoothing(double* z, int taille,double fs,int N,double ratio,int k);

#endif /* fonctions_h */
