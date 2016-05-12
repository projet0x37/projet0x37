#ifndef fonctions_h
#define fonctions_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../midi/midi.h"

#define F0 27.5000 // fréquence fondamentale utilisée pour calculer toutes le autres notes
#define OFFSETMIDI 21 // décalage du numéro des notes pour correspondre à la notation midi 
#define RATIOLISSAGE (double)1 // ratio pour le lissage
#define RATIOBANDE (double)2/3 // ratio pour le calcul des bandes passantes triangulaires
#define DELTAMIN 0.001 // permet d'ajouter une condition pour la boucle() , si vi n'est pas assez modifié , la boucle est arrétée
#define NBNOTES 88 // nombre de notes selon lesquelles des correspondances ( N°note <> fréquence ] s'effectuent
#define BMIN 100 //largeur minimale des portes triangulaires
#define FMIN 50 // fréquence minimale a partir de laquelle les traitements se font
#define FMAX 6000 // fréquence maximale en dessous de laquelle les traitements s'opèrent
#define BNMAX 17 // = floor(log(FMAX/FMIN)/log(4/3))+1 , ne pas oublier de l'ajuster si FMIN et FMAX sont modifiés

typedef double * frame;
int arraymultiplication( frame X, frame Y, int sizeframe , frame zb); // OK
int zeros( int l ,frame t); // OK
double * creer_notesBank( double samplerate , int sizeframe); //OK
double incertitude ( double knotes ); // OK
char correspondancenote( int kech , double * notesBank); 
double * Y_extraction(double * X, int taille, int k0 , int k1); //ok
int moving_average(double * Y, double ratio, int taille , frame movingA);//ok
double * Z_calc(double * Y, double * N, int taille);//ok
frame noisesup( frame X , int k0 , int k1 , int taille , double ratio, frame N);//ok
int processing_init( double Lmax , double SNR );
int iteration_checking( double Lmaxi , double SNR , double * vi);
double mean(double* T,double m0,double m2);//ok
double * npow( double* meanx, int k1,int k0,int taille);
double SNR_calc( frame X , frame N , int sizeframe, double k0, double k1 );
void functionBW (double * b , double kmin , int sizeframe , double kb , double * M0 , double * M2 , double ratio);//ok
double ** MatrixBW(int sizeframe , double k0 , double kmin , double * B_m0_m2);//ok
void Z_smoothing(double* z, int taille , int k , double kmin); // ok
void initTnote( Tnote T , int sizeTmax , int sizeframe , int samplerate); //OK
void short_time_DSP( frame x , int sizeframe, frame DSP); //OK
void lbvector(frame zb , int sizeframe , int KB , int kb , int U0 , double * Lb); // a priori Ok 
void Lvector( frame Z , int sizeframe , frame L ,double ** MatrixB , double * b_m0_m2); //OK
int boucle(chord * tabchord , frame Z , int sizeframe , double SNR , double kmin , double ** MatrixB , double * b_m0_m2 , double * NotesBank); // OK
void Hamming( frame x , int sizeframe ); // OK
int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, double kmin, double k0 , double k1 , double ** MatrixB, double * b_m0_m2 , double * NoteBank); // OK
double max_valueandposition_frame(frame X , int sizeframe , int * kmax); // OK
int zerostabofchar( int l ,char* t); // ok
notes * simplifT(Tnote T,int SIZE_T,double tmin); // ok
void mainprocessing( Tnote  T , int sizeTmax , double * datain , int sizedatain  , double samplerate , int sizeframe);

#endif /* fonctions_h */
