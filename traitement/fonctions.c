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
#define RATIOBANDE 0.666666667
#define BNMAX 17 // = floor(log(6000/50)/log(4/3))+1
#define DELTAMIN 0.00001
#define NBNOTES 128

double * creertab( double samplerate , int sizeframe) { // les valeurs stockées sont des fréquences adaptées  à la transformée de fourrier rapide fftw
	double * tab;
	double fo = F0;
	double k0 = (double)fo*sizeframe/samplerate
	int i;
	tab=calloc(NBNOTES,sizeof(*tab));
	for (i=0;i<NBNOTES;i++){
		tab[i]=k0*pow(2,(double)i/12);}
	return tab;
}


double incertitude ( double knotes ) {
	i = knotes*(pow(2,(float)1/12)-1))/2 ; // incertitude d'un quart de ton
	return i ;
}   

char correspondancenote( double kech , double * notesBank){
	int j;
	char note = -1;// valeur par défaut, si elle vaut -1 la correspondance note> fréquence n'a pas été concluante
	int n = NBNOTES;
	for(j=0;j<NBNOTES;j++) {
		if ( abs(notesBank[j]-kech) < incertitude(notesBank[j]) ){
			note = j + OFFSETMIDI; // + OFFSETMIDI pour s'adapter à la numérotation midi des notes
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


int iteration_checking( double Lmaxi , double SNR , double threshold ,double * vi){
	*vi=1.8*log(Lmaxi)-log(SNR);
	if (*vi>threshold) return 1;
	if (*vi<=threshold) return 0;
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
	
	if(N){
		SNR = x/N;
		return SNR;
	}
	else return 0;
	
}

/*
int F0fromL(double* L, int taille){ // remplacée par max_valueandposition_frame()
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
*/

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


void zeros( int l ,frame t){						//Testée et approuvée
	int i;	
	for(i=0;i<l;i++){
		t[i]=0;
	}
	return(t);
}


void functionBW (double * b , double kmin , int sizeframe , double kb , double * M0 , double * M2){   //Testée et approuvée
	int i;
	double kb1=round(kb);
	double m1,b1,a1,b2,a2;
	double ratio = RATIOBANDE;
	zeros(sizeframe,b);
	if (kb1*ratio > kmin) {
		*M0 = kb1;
		*M2 = *M0+(*M0)*ratio;
		m1 = (*M0+*M2)/2;
		b1 = *M0/(*M0-m1);
		a1 = 1/(m1-*M0);
		b2 = *M2/(*M2-m1);
		a2 = 1/(m1-*M2);
		i = *M0;
		while (i <= m1 && i<sizeframe){      
			b[i] = a1*i+b1;
			i=i+1;
		}
		while (i <= *M2 && i<sizeframe){
			b[i] = a2*i+b2;
			i=i+1;
		}
	}
	else {
		*M0 = kb1;
		m1 = kb1 + kmin/2;
		*M2 = kb1 + kmin;
		b1 = *M0/(*M0-m1);
		a1 = 1/(m1-*M0);
		b2 = *M2/(*M2-m1);
		a2 = 1/(m1-*M2);
		i = *M0;
		while (i <= m1 && i<sizeframe){	
			b[i] = a1*i+b1;	
			i=i+1;
		}
		while (i < *M2 && i<sizeframe){	
			b[i] = a2*i+b2;       
			i=i+1;
		}
	}
	*M2=i-1;
	return b ;
}


double ** MatrixBW(int sizeframe , int k0 , double kmin , double * B_m0_m2){
	double ** BankB;
	int i;
	int kb = k0;
	double * M0 = calloc(1,sizeof(double));
	double * M2 = calloc(1,sizeof(double));
	BankBW = calloc(BNMAX,sizeof(double *));
	BankBW[0] = calloc(BNMAX*sizeframe,sizeof(double)); // allocation contigue
	if(!BankBW || ! BankBW[0] ){
		puts("BankBW NULL");
		return NULL;
	}
	if(!B_m0,m2){
		puts("B_m0_m2 NULL");
		return NULL;
	}
	for(i=0;i < BNMAX;i++){ // on fait l'allocation et le calcul des passe-bandes triangulaires en même temps
		if(i>0){
			BankBW[i] = BankBW[i-1 + sizeframe ]
			kb=kb*4./3.
			functionBW(BankBW[i] , kmin , sizeframe , kb , M0 , M2);
			B_m0_m2[2*i] = floor(*M0);
			B_m0_m2[2*i+1] = floor(*M2);
		}
		}
		else{
			kb=kb*4./3.
			functionBW(BankBW[i] , kmin , sizeframe , kb , M0 , M2);
			B_m0_m2[0] = floor(*M0);
			B_m0_m2[1] = floor(*M2);
		}
	}
	
	return BankBW;
}


void Z_smoothing(double* z, int taille , int k){		//  NON Testée
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

		kb = i-i*RATIOLISSAGE/2;
		rb = 2*RATIOLISSAGE/(2-RATIOLISSAGE); // la fonction crée une porte triangulaire à partir de kb, ici on veut qu'elle soit centrée sur i, c'est pourquoi on fait intervenir un ratio "rb" différent de "ratio"
		b = functionBW(100,N,taille,kb,rb,M0,M2); // largeur minimale de 100hz ( à voir ) , fixé par kb*ratio sinon.

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
                		zsmoothed[j]=min;// a revoir, valable pour les moyennes et hautes fréquences , à ajuster pour les petites
			}
		}
		i=i+k;
		
	}
	for(i=0;i<taille;i++){
		z[i] = z[i]-zsmoothed[i];
	}
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


frame short_time_DSP( frame x , int sizeframe){ // il est possible d'optimiser l'utilisation des plans p, x doit étre alloué par fftw_malloc() de la librairie fftw
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

void lbvector(frame zb , int sizeframe , int KB , int kb , int U0 , double * Lb){
	int n0 = U0;
	int n1 = KB;
	int n;
	int k0;
	int k1;
	int lb = kb + KB -1;
	int J;
	double C;
	int m;
	int m0;
	int m1;
	double delta;
	int i;
	double Ln;
	int h;
	if(!Lb){
		puts("Lb NULL");
		return EXIT_FAILURE;
	}
	
	zeros(sizeframe,Lb);
	for(n=n0;n<n1;n++){
		m0 = (floor(kb/n)+1)*n -kb;
		delta = (double)lb/n;
		delta *= delta;
		delta = lb*(sqrt(1+0.01*(delta-1))-1)
		m1 = m0 + round(delta);
		if(m1>m0+n-1){
			m0=0;
			m1=n-1;
		}
		for(m=m0;m<m1;m++){
			J = floor((Kb-m-1)/n)+1
			C = 0.75/J + 0.25;
			Ln=0;
			for(i=0; i < J && kb+m+n*i < sizeframe){
				Ln=Ln+c*zb[kb+m+n*i];
				i++;
			}
			if(Lb[n] < Ln) Lb(n) = Ln;
		}
	}
	h=1;
	k0 = floor((kb+KB)/(h+1));
	if(k0<kb) k0=kb;
	k1 = kb + KB -1;
	while(k0<=k1){
		for(i=k0;i<=k1;i++){
			n=round(i/h);
			if(k<sizeframe){
				if(Lb[n] < zb[i]) Lb[n] = zb[i];
			}
		}
		h++;
		k0 = floor((kb+KB)*h/(h+1))+1
		if(k0<kb) k0 = kb;
		k1=floor((kb-1)*h/(h-1))
		if(k1>kb+KB) k1 = kb + KB;
		
	}
}


void Lvector( frame Z , int sizeframe , int kmin , frame L ,double ** MatrixB , double * b_m0_m2){
	int m0;
	int m2;
	int i,j;
	int KB;
	int k0 = b_m0_m2[0];
	frame zb = calloc(sizeframe,sizeof(double));
	frame lb = calloc(sizeframe,sizeof(double));
	zeros(sizeframe,L);
	if( !L || !MatrixB || !b_m0_m2){
		puts("prob lvector()");
		return EXIT_FAILURE;
	}
	for(i=0 ; i<BNMAX ; i++){
		arraymultiplication( Z , MatrixB[i] , sizeframe , zb);
		m0 = b_m0_m2[2*i];
		m2 = b_m0_m2[2*i+1];
		KB = m2 - m0 + 1;
		lbvector( zb ,  sizeframe , KB , m0 , k0 ,  Lb);
		for(j=0;j<sizeframe;j++)L[j]=L[j] + Lb[j];
	}
}
	


int boucle(chord * tabchord , frame Z , int sizeframe , double SNR , int kmin , double thresv0 , double thresvi , int k0 , int k1, double ** MatrixB , double * b_m0_m2 , double * NotesBank){
	int b=1;
	frame L;
	int i=0;
	int j;
	double Lmax;
	int ksup;
	int ks;
	int  kmax;
	double Lmax;
	double v;
	double deltavi;
	double vi;
	int itcheck;
	L=calloc(sizeframe,sizeof(double));
	while(b>0){
		Lvector(Z,sizeframe,kmin,L,MatrixB,b_m0_m2);
		if(i!=0){
			for(j=0;j<i;j++){
				ksup = tabchord[j].note;
				ksup0 = ceil(ksup-pow(ksup,(double)1/12)/2);
				ksup1 = floor(ksup + pow(ksup,(double)1/12)/2);
				for(ks=ksup0;ks<=ksup1;ks++)L(ks)=0;
			}
		}
		Lmax = max_valueandposition_frame(L,sizeframe, &kmax);
		
		if(i==1){
			if(processing_init( Lmax , SNR , thresv0) == 1){
				tabchord[i].note = correspondancenote( kmax , notesBank )
				Z_smoothing( Z, sizeframe , kmax);
				i++;
			}
			else b=0;
		}
		else{
			deltavi = vi;
			itcheck = iteration_checking( Lmax , SNR ,  thresvi , &vi)
			deltavi = abs( deltavi - vi );
			if( itcheck == 1 && i < 10 && deltavi > DELTAMIN ){
				tabchord[i].note = correspondancenote( kmax , notesBank )
				Z_smoothing( Z, sizeframe , kmax);
				i++;
			}
			else b = 0;
		}
	}
	return i-1;
}		
	

int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, int kmin, int k0 , int k1 , double ** MatrixB, double * b_m0_m2 , double * NoteBank){
	frame X;
	frame N;
	frame Z;
	double SNR;
	int b=0;
	X = short_time_DSP( x , sizeframe );
	if(!X ){
		printf("frameprocessing() X NULL\n");
		return 0;
	}
	N = calloc(sizeframe, sizeof(double));
	SNR = SNR_calc( X , N , sizeframe);
	Z  = noisesup( X , k0 , k1 , sizeframe , (double)2./3. , N );

	free(X); // une fois que le rapport signal sur bruit (SNR) est calculé, N et X sont inutiles
	free(N);

	b=boucle(tabchord,Z,sizeframe,SNR,kmin,thresv0,thresvi,k0,k1,MatrixB,b_m0,m2);
	if(b==0) return 0; // 0 si rien n'a été modifié dans tabchord
	else return 1; // 1 si on a au moins une note, en sachant que b représente le nombre de notes détectées
}
	
	
double max_valueandposition_frame(frame X , int sizeframe , int * kmax){
	int i;
	double M=-1;
	if( !X ){
		printf(" frame X est NULL\n");
		return 0;
	}
	for(i=0;i<sizeframe;i++){
		if(X[i] > M){
			if(!kmax) *kmax = i;
			M = X[i];
		}
	}
	return M;
}


void arraymultiplication( frame X, frame Y, int sizeframe , frame zb){
	int i;
	if(!X || !Y || !zb){
		puts("problème arraymultiplication");
		return NULL;
	}
	for(i=0;i<sizeframe;i++){
		zb[i] = X[i]*Y[i];
	}
}


