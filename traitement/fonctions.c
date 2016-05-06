#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "fonctions.h"
#include "../textexport/textexport.h"
#include "../midi/midi.h"
#include <fftw3.h>


#define F0 27.5000
#define OFFSETMIDI 21
#define RATIOLISSAGE 1
#define RATIOBANDE 2./3.
double * creertab( int n) {
	double * tab;
	double fo = F0;
	int i;
	tab=calloc(n,sizeof(*tab));
	for (i=0;i<n;i++){
		tab[i]=fo*pow(2,(double)i/12);}
	return tab;}
  
double incertitude ( int n ) {
	double fo= F0;
	double i ;
	i= (fo*pow(2,(float)n/12)*(pow(2,(float)1/12)-1))/2 ;
	return i ;
	}

    


char correspondancenote( float fech , int n ){
	int j=0;
	char note = -1;// valeur par défaut, si elle vaut -1 la correspondance note> fréquence n'a pas été concluante
	double * tab = creertab(128) ;
	for(j=0;j<128;j++) {
		if ( abs(tab[j]-fech) < incertitude(j) ){
			note = j;
		}
	}
	return note;
}

double * Y_extraction(double * X, int taille, int k0 , int k1){
	int l = k0;
	double g, somme = 0;
	int i;
	double * Y = NULL;

	while(l<k1){
		somme += pow(X[l],1/3);
		l += 1;
	}

	somme = somme/(k1-k0+1);
	g = pow(somme,3);

	printf("g = %lf\n",g);

	Y = calloc(taille, sizeof(*Y));

	for(i=0;i<taille;i++){
		Y[i] = log(1 + X[i]/g);
	
	}
	return Y;
	
}

void moving_average(double * Y, double ratio, int taille , frame movingA){
	double localA = 0;
	int i,j;
	for(i=0;i<=taille;i++){
		if(i-ratio*i/2 < 0 || i+ratio*i/2 > taille){
			movingA[i] = Y[i];
		}
		else{
			localA = 0;
			for( j=i-round(ratio*i/2) ; j<=i+round(ratio*i/2) ; j++ ){
				localA += Y[j];
			}
		movingA[i] = localA/(ratio*i);
		}
	}
	return movingA;

}

double * Z_calc(double * Y, double * N, int taille){
	int i;
	double * Z = calloc(taille,sizeof(*Z));
	if(!Z) return NULL;
	for(i=0;i<taille;i++){
		Z[i] = ((Y[i]-N[i])> 0) ? Y[i]-N[i] : 0;
	}
	return Z;
}

frame noisesup( frame X , int k0 , int k1 , int taille , double ratio, frame N){
	frame Y;
	frame Z;
	Y = Y_extraction( X , taille , k0 , k1);
	moving_average( Y , ratio , taille , N);
	Z = Z_calc( Y , N , taille );
	free(Y);
	return Z;
}

int processing_init( double Lmax , double SNR , double threshold ){
	double v0=0;
	v0=4*log(Lmax)+log(SNR);
	if (v0>threshold) return 1;
	if (v0<=threshold) return 0;
	return -1;
}

int iteration_checking( double Lmaxi , double SNR , double threshold ){
	double vi=0;
	vi=1.8*log(Lmaxi)-log(SNR);
	if (vi>threshold) return 1;
	if (vi<=threshold) return 0;
	return -1;
}

double SNR_calc( frame X , frame N , int sizeframe, int k0, int k1 ){
	frame Npow=npow(N,k1,k0,sizeframe);
	double x=0;
	double N=0;
	double SNR;
	for(i=k0;i<k1;i++){
		x+=X[i] ;
		N+=Npow[i];
		}
	
	if(N)SNR = x/N;
	else return 0;
	return SNR;
}

int F0fromL(double* L, int taille){			//Testée et approuvée
	int i;
	double M=-1;
	int fM;
	if(!L){
		puts("Erreur : L est vide");
		return(0);
	}
	for(i=0;i<taille;i++){
		if(L[i]>M){
			M = L[i];
			fM = i;
		}
	}
	return(fM);
}

double mean(double* T,double m0,double m2){   //Calcul la moyenne entre m0 et m2 //Testée et Approuvée
	double S = 0;
	int i;
	if(!T){
		puts("Erreur");
		return(0);
	}
	if(m0<0){
		puts("Erreur");
		return(0);
	}
	for(i=(int)m0;i<(int)m2+1;i++){
		S=S+T[i];
	}
	return(S/(m2-m0+1));
}


 
int round(double value) {					//Testée et approuvée
     return floor(value + 0.5);
}
 
double* zeros(int l){						//Testée et approuvée
	int i;
	double* t = calloc(l,sizeof(*t));
	for(i=0;i<l;i++){
		t[i]=0;
	}
	return(t);
}

double* functionBW (double kmin,int N, double fs, int l, double kb, double* M0, double* M2){   //Testée et approuvée
	int i;
	double* b = zeros(l);
	double kb1=round(kb);
	double m1,b1,a1,b2,a2 ;
	double ratio=RATIOBANDE;
	if (kb1*ratio > kmin) {
		*M0 = kb1;
		*M2 = *M0+(*M0)*ratio;
		m1 = (*M0+*M2)/2;
		b1 = *M0/(*M0-m1);
		a1 = 1/(m1-*M0);
		b2 = *M2/(*M2-m1);
		a2 = 1/(m1-*M2);
		i = *M0;
		while (i <= m1 && i<l){      
			b[i] = a1*i+b1;
			i=i+1;
		}
		while (i <= *M2 && i<l){
			b[i] = a2*i+b2;
			i=i+1;
		}
	}
	else {
		*M0 = kb1;
		m1 = kb1 + kmin/2;
		printf("m1 = %lf\n",m1);
		*M2 = kb1 + kmin;
		b1 = *M0/(*M0-m1);
		a1 = 1/(m1-*M0);
		b2 = *M2/(*M2-m1);
		a2 = 1/(m1-*M2);
		i = *M0;
		while (i <= m1 && i<l){	
			b[i] = a1*i+b1;	
			i=i+1;
		}
		while (i < *M2 && i<l){	
			b[i] = a2*i+b2;       
			i=i+1;
		}
	}
	*M2=i-1;
	return b ;
}

double* Z_smoothing(double* z, int taille,double fs,int N,double ratio,int k){		//  NON Testée
	double * zsmoothed = calloc(taille,sizeof(*zsmoothed));
	int i = k;
	double nfactor;		//facteur permettant de réhausser la moyenne pondérée (on suppose que les fondamentales sont détectées de manière croissante : il 						n'y à alors pas d'inconvénient à supprimer celle détectée).
	double* M0 = calloc(1,sizeof(*M0));
	double* M2 = calloc(1,sizeof(*M2));
	double mz;
	double mb;
	double m;
	double min;
	int j;
	int j0;
       	int j1;
	double* zb = calloc(taille,sizeof(*zb));
	double kb;
	double rb;
	double* b = calloc(taille,sizeof(*b));

	while(i+k < taille){

		kb = i-i*ratio/2;
		rb = 2*ratio/(2-ratio); // la fonction crée une porte triangulaire à partir de kb, ici on veut qu'elle soit centrée sur i, c'est pourquoi on fait intervenir un ratio "rb" différent de "ratio"
		b = functionBW(100,N,fs,taille,kb,rb,M0,M2); // largeur minimale de 100hz ( à voir ) , fixé par kb*ratio sinon.

		for(i=0;i<taille;i++){
			zb[i]=z[i]*b[i];
		}

		mz = mean(zb,*M0,*M2);
		mb = mean(b,*M0,*M2);
		m = mz/mb;

		if(i==k && m!=0){
			nfactor = (double)z[i]/m;
		}

		m = m*nfactor;
        	j0 = floor(i-pow(i,1/12)*(3/4));
       		j1 = floor(i+pow(i,1/12)*(3/4))+1;

		for(j=j0;j<j1+1;j++){
            		if(fabs(j-i)<4){
				min = m;
				if(zb[j]<m){
					min = zb[j];
				}
                		zsmoothed[j]=min; 	  // a revoir , valable pour les moyennes hautes fréquences , à ajuster pour les petites
			}
		}
		i=i+k;
		
	}
	for(i=0;i<taille;i++){
		zsmoothed[i] = z[i]-zsmoothed[i];
	}
	return(zsmoothed);
}






double * npow( double* meanx, int k1,int k0,int taille){
	int i;
	double g=0;
	double* Npow=zeros(taille);
	for (i=k0;i<k1;i++){
		g+=pow(meanx[i],(double)1/3);
	}
	g=g/(k1-k0+1);
	g=pow(g,3);
	for(i=k0;i<k1;i++){
		Npow[i]=exp(meanx[i]-1)*g;
	}
	return Npow;
}



void initTnote( Tnote T , int sizeTmax){
	int i;
	if(!T) return EXIT_FAILURE;
	for(i=0;i<sizeTmax;i++){
		Tnote[i].temps=-1.0
	}
}

frame short_time_DSP( frame x , int sizeframe){ // il est possible d'optimiser l'utilisation des plans p
	fftw_complex * out;
	fftw_plan p;
	frame DSP;
	if( !x ){
		printf("short_time_DSP() frame x = NULL\n");
		return NULL;
	}
	
	sizeout = sizeframe/2 + 1;
	out = fftw_malloc(sizeof(fftw_complex)*sizeout);
	p = fftw_plan_dft_r2c_1d( sizeframe , x , out , FFTW_ESTIMATE );

	DSP=calloc(sizeout,sizeof(double));
	
	fftw_execute(p);	
	
	for(i=0;i<sizeout;i++) DSP[i] = ( out[i][0]*out[i][0] + out[i][1]*out[i][1] );
	
	fftw_free(out);
	fftw_destroy_plan(p);
	
	return DSP;
}
int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, int kmin, int k0 , int k1){
	frame X;
	frame N;
	frame Z;
	double SNR;
	int b;
	int nmax = floor(log(6000/50)/log(4/3))+1;
	
	X = short_time_DSP( x , sizeframe );
	if(!X ){
		printf("frameprocessing() X NULL\n");
		return 0;
	}
	N = calloc(sizeframe, sizeof(double));
	SNR = SNR_calc( X , N , sizeframe);
	Z  = noisesup( X , k0 , k1 , sizeframe , (double)2/3 , N );
	free(X); // une fois que le rapport signal sur bruit (SNR) st calculé on a plus besoin de N et X
	free(N);
	b=boucle(tabchord,Z,sizeframe,SNR,nmax,kmin,thresv0,thresvi,k0,k1);
		
		
	
	
double max_valueandposition_frame(frame X , int sizeframe , int * kmax){
	int i;
	double M=-1;
	if( !X ){
		printf(" frame X est NULL\n");
		return 0;
	}
	for(i=0;i<sizeframe;i++){
		if(X[i] > M){
			if(!kmax)*kmax=i;
			M=X[i];
		}
	}
	return M;
}

frame arraymultiplication( frame X, frame Y, int sizeframe){
	int i;
	frame Z=calloc(sizeframe,sizeof(double));
	if(!X || !Y || !Z){
		puts("problème arraymultiplication");
		return NULL;
	}
	for(i=0;i<sizeframe;i++){
		Z[i] = X[i]*Y[i];
	}
	return Z;
}


