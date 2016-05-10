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
#define DELTAMIN 0.00001 // permet d'ajouter une condition pour la boucle() , si vi n'est pas assez modifié , la boucle est arrétée
#define NBNOTES 128 // nombre de notes selon lesquelles des correspondances ( N°note <> fréquence ] s'effectuent
#define BMIN 100 //largeur minimale des portes triangulaires
#define FMIN 50 // fréquence minimale a partir de laquelle les traitements se font
#define FMAX 6000 // fréquence maximale en dessous de laquelle les traitements s'opèrent
#define BNMAX 17 

typedef double * frame;
int arraymultiplication( frame X, frame Y, int sizeframe , frame zb); // OK
int zeros( int l ,frame t); // OK
double * creer_notesBank( double samplerate , int sizeframe); //OK
double incertitude ( double knotes ); // OK
char correspondancenote( double kech , double * notesBank); 
double * Y_extraction(double * X, int taille, int k0 , int k1); //ok
int moving_average(double * Y, double ratio, int taille , frame movingA);//ok
double * Z_calc(double * Y, double * N, int taille);//ok
frame noisesup( frame X , int k0 , int k1 , int taille , double ratio, frame N);//ok
int processing_init( double Lmax , double SNR , double threshold );
int iteration_checking( double Lmaxi , double SNR , double threshold ,double * vi);
double mean(double* T,double m0,double m2);//ok
double * npow( double* meanx, int k1,int k0,int taille);
double SNR_calc( frame X , frame N , int sizeframe, int k0, int k1 );
void functionBW (double * b , double kmin , int sizeframe , double kb , double * M0 , double * M2 , double ratio);//ok
double ** MatrixBW(int sizeframe , double k0 , double kmin , double * B_m0_m2);//ok
void Z_smoothing(double* z, int taille , int k , int kmin);
void initTnote( Tnote T , int sizeTmax , int sizeframe , int samplerate); 
void short_time_DSP( frame x , int sizeframe, frame DSP);
void lbvector(frame zb , int sizeframe , int KB , int kb , int U0 , double * Lb);
void Lvector( frame Z , int sizeframe , int kmin , frame L ,double ** MatrixB , double * b_m0_m2);
int boucle(chord * tabchord , frame Z , int sizeframe , double SNR , int kmin , double thresv0 , double thresvi , int k0 , int k1, double ** MatrixB , double * b_m0_m2 , double * NotesBank);
void Hamming( frame x , int sizeframe );
int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, int kmin, int k0 , int k1 , double ** MatrixB, double * b_m0_m2 , double * NoteBank, double * thresv0 , double * thresvi);
double max_valueandposition_frame(frame X , int sizeframe , int * kmax);
int zerostabofchar( int l ,char* t);
notes * simplifT(Tnote T,int SIZE_T,double tmin);
void mainprocessing( Tnote  T , int sizeTmax , double * datain , int sizedatain  , double samplerate , int sizeframe);

#endif /* fonctions_h */
