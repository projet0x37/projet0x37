#ifndef fonctions_h
#define fonctions_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../midi/midi.h"

typedef double * frame;
int arraymultiplication( frame X, frame Y, int sizeframe , frame zb);
int zeros( int l ,frame t);
double * creer_notesBank( double samplerate , int sizeframe);
double incertitude ( double knotes );
char correspondancenote( double kech , double * notesBank);
double * Y_extraction(double * X, int taille, int k0 , int k1);
int moving_average(double * Y, double ratio, int taille , frame movingA);
double * Z_calc(double * Y, double * N, int taille);
frame noisesup( frame X , int k0 , int k1 , int taille , double ratio, frame N);
int processing_init( double Lmax , double SNR , double threshold );
int iteration_checking( double Lmaxi , double SNR , double threshold ,double * vi);
double mean(double* T,double m0,double m2);
double * npow( double* meanx, int k1,int k0,int taille);
double SNR_calc( frame X , frame N , int sizeframe, int k0, int k1 );
void functionBW (double * b , double kmin , int sizeframe , double kb , double * M0 , double * M2);
double ** MatrixBW(int sizeframe , int k0 , double kmin , double * B_m0_m2);
void Z_smoothing(double* z, int taille , int k , int kmin);
void initTnote( Tnote T , int sizeTmax);
frame short_time_DSP( frame x , int sizeframe);
void lbvector(frame zb , int sizeframe , int KB , int kb , int U0 , double * Lb);
void Lvector( frame Z , int sizeframe , int kmin , frame L ,double ** MatrixB , double * b_m0_m2);
int boucle(chord * tabchord , frame Z , int sizeframe , double SNR , int kmin , double thresv0 , double thresvi , int k0 , int k1, double ** MatrixB , double * b_m0_m2 , double * NotesBank);
int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, int kmin, int k0 , int k1 , double ** MatrixB, double * b_m0_m2 , double * NoteBank, double * thresv0 , double * thresvi);
double max_valueandposition_frame(frame X , int sizeframe , int * kmax);
void mainprocessing( Tnote  T , int sizeTmax , double * datain , int sizedatain  , double samplerate , int sizeframe);

#endif /* fonctions_h */
