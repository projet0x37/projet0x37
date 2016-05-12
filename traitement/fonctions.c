#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "fftw3.h"
#include "fonctions.h"
#include "../textexport/textexport.h"
#include "../midi/midi.h"

extern double facteurmoyenne;
extern double thresv0;
extern double thresvi;


int arraymultiplication( frame X, frame Y, int sizeframe , frame zb){ //OK
	int i;
	if(!X || !Y || !zb){
		puts("erreur arraymultiplication()");
		return 0;
	}
	else{
		for(i=0;i<sizeframe;i++)zb[i] = X[i]*Y[i];
		return 1;
	}
}

int zeros( int l ,frame t){ // OK
	int i;
	if(!t){
		puts("erreur zeros()");
		return 0;
	}
	else{
		for(i=0;i<l;i++)t[i]=0;
		return 1;
	}
}


double * creer_notesBank( double samplerate , int sizeframe){ //OK// les valeurs stockées sont des fréquences adaptées  à la transformée de fourrier rapide fftw
	double * tab;
	double k0 = (double)F0*sizeframe/samplerate;
	int i;
	tab=calloc(NBNOTES,sizeof(double));
	for (i=0;i<NBNOTES;i++){
		tab[i]=k0*pow(2,(double)i/12);}
	return tab;
}


double incertitude ( double knotes ){ // OK
	double i;
	i = knotes*(pow(2,(float)1/12)-1)/2; // incertitude d'un quart de ton , peut être un peu large , cela autorise une imprécision de 6 %
	return i ;
}   


char correspondancenote( int kech , double * notesBank){ // OK // les notes midi vont de 21 à 108 pour un piano ( 88 touches )
	int j;
	char note = 0;// valeur par défaut, si elle vaut -1 la correspondance note> fréquence n'a pas été concluante
	for(j=0;j<NBNOTES;j++) {
		if ( abs(notesBank[j]-kech) < incertitude(notesBank[j]) ){
			note = j + OFFSETMIDI; // + OFFSETMIDI pour s'adapter à la numérotation midi des notes
		}
	}
	return note;
}


double * Y_extraction(double * X, int taille, int k0 , int k1){ // OK
	int l = k0;
	double g, somme = 0;
	int i;
	double * Y = NULL;

	while(l<k1){
		somme += pow(X[l],(double)1/3);
		l += 1;
	}

	somme = (double)somme/(k1-k0+1);
	g = pow(somme,3);

	

	Y = calloc(taille, sizeof(*Y));

	for(i=0;i<taille;i++){
		Y[i] = log(1 + (double)X[i]/g);
	
	}
	return Y;
	
}


int moving_average(double * Y, double ratio, int taille , frame movingA){ // OK
	double localA = 0;
	int i,j;
	if(!Y || !movingA){
		puts("erreur moving_average");
		return 0;
	}
	else{
		for(i=0;i<=taille;i++){
			if(i-ratio*i/2 < 0 || i+ratio*i/2 > taille){
				movingA[i] = Y[i];
			}
			else{
				localA = 0;
				for( j=i-round(ratio*i/2) ; j<=i+round(ratio*i/2) ; j++ ){
					localA += Y[j];
				}
			movingA[i] = facteurmoyenne*localA/(ratio*i);
			}
		}
		return 1;
	}

}


double * Z_calc(double * Y, double * N, int sizeframe){ // ok
	int i;
	double * Z = calloc(sizeframe,sizeof(*Z));
	if(!Z) return NULL;
	for(i=0;i<sizeframe;i++){
		Z[i] = ((Y[i]-N[i])> 0) ? Y[i]-N[i] : 0;
	}
	return Z;
}


frame noisesup( frame X , int k0 , int k1 , int sizeframe , double ratio, frame N){ // ok
	frame Y;
	frame Z;
	Y = Y_extraction( X , sizeframe , k0 , k1);
	moving_average( Y , ratio , sizeframe , N);
	Z = Z_calc( Y , N , sizeframe );
	return Z;
}


int processing_init( double Lmax , double SNR){
	double v0=0;
	v0=4*log(Lmax)+log(SNR);
	if (v0>thresv0) return 1;
	if (v0<=thresv0) return 0;
	return -1;
}


int iteration_checking( double Lmaxi , double SNR ,double * vi){
	*vi=1.8*log(Lmaxi)-log(SNR);
	if (*vi>thresvi) return 1;
	if (*vi<=thresvi) return 0;
	return -1;
}


double mean(double* T,double m0,double m2){   //OK
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


double * npow( double* meanx, int k1,int k0,int taille){
	int i;
	double g=0;
	double* Npow=calloc(taille,sizeof(double));
	if(!zeros(taille,Npow) || !meanx){
		puts("erreur npow()");
		return NULL;
	}
	else{
		for (i=k0;i<k1;i++){
			g+=pow(meanx[i],(double)1./3.);
		}
		g=g/(k1-k0+1);
		g=pow(g,3);
		for(i=k0;i<k1;i++){
			Npow[i]=(exp(meanx[i])-1)*g;
		}
		return Npow;
	}
}

double SNR_calc( frame X , frame N , int sizeframe, double k0, double k1 ){
	frame Npow;;
	double sumx=0;
	double sumNpow=0;
	double SNR;
	int i;
	int i0=round(k0);
	int i1=round(k1);
	Npow = npow(N,i1,i0,sizeframe);
	if(!X || !N || !Npow){
		puts("erreur SNR_calc");
		return -1;
	}
	for( i=i0 ; i < i1 ; i++ ){
		sumx+=X[i] ;
		sumNpow+=Npow[i];
	}
	
	free(Npow);
	if(N>0){
		SNR = sumx/sumNpow;
		return SNR;
	}
	else{
		puts("erreur SNR_calc");
		return -1;
	}
}
 

void functionBW ( double * b , double kmin , int sizeframe , double kb , double * M0 , double * M2, double ratio ){   //OK
	int i;
	double kb1 = kb;
	double m1,b1,a1,b2,a2;
	
	if(!b || !M0 || !M2){
		puts("erreur functionBW");
		return;
	}

	zeros(sizeframe,b);

	if (kb1*ratio > kmin) {
		*M0 = kb1;
		*M2 = *M0+(*M0)*ratio;
		m1 = (*M0+*M2)/2.;
		b1 = *M0/(*M0-m1);
		a1 = 1./(m1-*M0);
		b2 = *M2/(*M2-m1);
		a2 = 1./(m1-*M2);
		i = floor(*M0)+1;
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
		i = floor(*M0)+1;
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
}


double ** MatrixBW(int sizeframe , double k0 , double kmin , double * B_m0_m2){ // OK
	double ** BankBW;
	int i;
	double kb = k0;
	double * M0 = calloc(1,sizeof(double));
	double * M2 = calloc(1,sizeof(double));

	BankBW = calloc(BNMAX,sizeof(double *));
	BankBW[0] = calloc(BNMAX*sizeframe,sizeof(double)); // allocation contigue

	if(!BankBW || ! BankBW[0] ){
		puts("BankBW NULL");
		return NULL;
	}
	if(!B_m0_m2){
		puts("B_m0_m2 NULL");
		return NULL;
	}
	for(i=0;i < BNMAX;i++){ // on fait l'allocation et le calcul des passe-bandes triangulaires en même temps
		if(i>0){
			BankBW[i] = BankBW[i-1] + sizeframe ;
			kb = (double)kb*4./3.;
			functionBW(BankBW[i] , kmin , sizeframe , kb , M0 , M2, RATIOBANDE);
			B_m0_m2[2*i] = *M0;
			B_m0_m2[2*i+1] = *M2;
		}
		else{
			functionBW(BankBW[i] , kmin , sizeframe , kb , M0 , M2, RATIOBANDE);
			B_m0_m2[0] = *M0;
			B_m0_m2[1] = *M2;
		}
	}
	
	return BankBW;
}


void Z_smoothing(double* z, int taille , int k , double kmin){		//  OK
	double * zsmoothed = calloc(taille,sizeof(*zsmoothed));
	int i = k;
	double nfactor;		//facteur permettant de réhausser la moyenne pondérée (on suppose que les fondamentales sont détectées de manière croissante : il 						n'y à alors pas d'inconvénient à supprimer celle détectée).
	double* M0 = calloc(1,sizeof(double));
	double* M2 = calloc(1,sizeof(double));
	double mz;
	double mb;
	double m;
	double min;
	int j;
	int j0;
       	int j1;
	double* zb = calloc(taille,sizeof(double));
	double kb;
	double rb;
	double* b = calloc(taille,sizeof(double));

	while(i+k < taille){

		kb = i-i*RATIOLISSAGE/2;
		rb = 2*RATIOLISSAGE/(2-RATIOLISSAGE); // la fonction crée une porte triangulaire à partir de kb, ici on veut qu'elle soit centrée sur i, c'est pourquoi on fait intervenir un ratio de lissage différent du ratio qu'on utilise pour le traitement du bruit
		functionBW(b,kmin,taille,kb,M0,M2,rb); // largeur minimale de 100hz ( à voir ) , fixé par kb*ratio sinon.

		for(j=0;j<taille;j++){
			zb[j]=z[j]*b[j];
		}

		mz = mean(zb,*M0,*M2);
		mb = mean(b,*M0,*M2);
		m = mz/mb; // moyenne pondérée ici

		if(i==k && m!=0) nfactor = (double)z[i]/m;
		m = m*nfactor;
        	j0 = floor(i-pow(i,1/12)*(3/4));
       		j1 = floor(i+pow(i,1/12)*(3/4))+1;

		for(j=j0;j<j1+1;j++){
            		if(abs(j-i)<4){
				min = m;
				if(zb[j]<m) min = zb[j];
                		zsmoothed[j]=min;// a revoir, valable pour les moyennes et hautes fréquences , à ajuster pour les petites
			}
		}
		i=i+k;
		
	}
	for(i=0;i<taille;i++){
		z[i] = z[i]-zsmoothed[i];
	}
}




void initTnote( Tnote T , int sizeTmax , int sizeframe , int samplerate){
	int i;
	int j;
	double tmin=(double)sizeframe/samplerate;
	if(!T){
		puts("erreur initTnote()");		
		return;
	}
	for(i=0;i<sizeTmax;i++){
		T[i].temps=0 ;
		for(j=0;j<SIZE_TABCHORD;j++){
			T[i].tabchord[j].note = (char)-1 ;
			T[i].tabchord[j].duree = tmin ;
			
		}
	}
	
}


void short_time_DSP( frame x , int sizeframe , frame DSP){ // OK// il est possible d'optimiser l'utilisation des plans p, x doit étre alloué par fftw_malloc() de la librairie fftw
	fftw_complex * out;
	fftw_plan p;
	int sizeout;
	int i;
	if( !x ){
		printf("short_time_DSP() frame x = NULL\n");
		return;
	}
	
	sizeout = sizeframe/2 + 1;
	out = fftw_malloc(sizeof(fftw_complex)*sizeout);
	p = fftw_plan_dft_r2c_1d( sizeframe , x , out , FFTW_ESTIMATE );
	
	fftw_execute(p);	
	
	for(i=0;i<sizeout;i++) DSP[i] = ( out[i][0]*out[i][0] + out[i][1]*out[i][1] );
	
	fftw_free(out);
	fftw_destroy_plan(p);
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
		return;
	}
	
	zeros(sizeframe,Lb);
	for(n=n0;n<n1;n++){
		m0 = (floor(kb/n)+1)*n -kb;
		delta = (double)lb/n;
		delta *= delta;
		delta = lb*(sqrt(1+0.01*(delta-1))-1);
		m1 = m0 + round(delta);
		if(m1>m0+n-1){
			m0=0;
			m1=n-1;
		}
		for(m=m0;m<m1;m++){
			J = floor((KB-m-1)/n)+1;
			C = 0.75/J + 0.25;
			Ln=0;
			for(i=0; i < J && kb+m+n*i < sizeframe;i++){
				Ln=Ln+C*zb[kb+m+n*i];
			}
			if(Lb[n] < Ln) Lb[n] = Ln;
		}
	}
	h=1;
	k0 = floor((kb+KB)/(h+1));
	if(k0<kb) k0=kb;
	k1 = kb + KB -1;
	while(k0<=k1){
		for(i=k0;i<=k1;i++){
			n=round(i/h);
			if(i<sizeframe){
				if(Lb[n] < zb[i]) Lb[n] = zb[i];
			}
		}
		h++;
		k0 = floor((kb+KB)*h/(h+1))+1;
		if(k0<kb) k0 = kb;
		k1 = floor((kb-1)*h/(h-1));
		if(k1>kb+KB) k1 = kb + KB;
		
	}
}


void Lvector( frame Z , int sizeframe , frame L ,double ** MatrixB , double * b_m0_m2){
	int m0;
	int m2;
	int i,j;
	int KB;
	int k0 = round(b_m0_m2[0]);
	frame zb = calloc(sizeframe,sizeof(double));
	frame lb = calloc(sizeframe,sizeof(double));
	zeros(sizeframe,L);
	if( !L || !MatrixB || !b_m0_m2){
		puts("erreur lvector()");
		return;
	}
	for(i=0 ; i<BNMAX ; i++){
		arraymultiplication( Z , MatrixB[i] , sizeframe , zb);
		m0 = round(b_m0_m2[2*i]);
		m2 = round(b_m0_m2[2*i+1]);
		KB = m2 - m0 + 1;
		lbvector( zb ,  sizeframe , KB , m0 , k0 ,  lb);
		for(j=0;j<sizeframe;j++)L[j]=L[j] + lb[j];
	}
}
	


int boucle(chord * tabchord , frame Z , int sizeframe , double SNR , double kmin , double ** MatrixB , double * b_m0_m2 , double * NotesBank){ // OK
	int b=1;
	frame L;
	int i = 0;
	int j;
	int ksup;
	int ksup0;
	int ksup1;
	int ks;
	int  kmax;
	double Lmax;
	double deltavi;
	double vi;
	int itcheck;

	L=calloc(sizeframe,sizeof(double));
	if(!L){
		puts("erreur allocation L boucle()");
		return 0;
	}
	i=0;
	b=1;
	kmax=0;
	while(b>0){
		Lvector(Z,sizeframe,L,MatrixB,b_m0_m2);
		if(i!=0){ // En partant du principe que les fréquences fondamentales les plus basses sont les premières détectées avec cette méthode, on supprime du vecteur L les fondamentales des notes précédement trouvées
			for(j=0;j<i;j++){
				ksup = tabchord[j].kech;
				
				ksup0 = ceil(ksup-pow(ksup,(double)1/12)/2); // on s'autorise une largeur d'un quart de ton autour de la note , ~ 6 % de marge
				ksup1 = floor(ksup + pow(ksup,(double)1/12)/2);
				for(ks=ksup0;ks<=ksup1;ks++)L[ks]=0;
			}
		}
		Lmax = max_valueandposition_frame(L,sizeframe, &kmax); // on calcul le maximum Lmax et sa position kmax dans L
		if(i==0){
			if(processing_init( Lmax , SNR) == 1){ // si Lmax est suffisament grand par rapport à v0 alors on stocke la note
				tabchord[i].kech = kmax;
				tabchord[i].note = correspondancenote( (int)kmax , NotesBank ); //stockage
				Z_smoothing( Z, sizeframe , kmax , kmin); // on lisse le spectre pour supprimer la fréquence fondamentale détectée
				
				i++;
			}
			else b = 0;
		}
		else{
			deltavi = vi; // deltavi permet de quantifier la modification du vecteur L en conséquence du lissage de Z, si deltavi ne varie plus on suppose qu'il n'y a plus rien a détecter, cela evite de faire tourner la boucle pour rien
			itcheck = iteration_checking( Lmax , SNR , &vi); // condition sur le seuil thresvi , à mesure qu'on supprime du spectre Z des informations, Lmax diminue , s'il est trop faible on ne stocke aucune information
			deltavi = abs( deltavi - vi );
			if( itcheck == 1 && i < 11 && deltavi > DELTAMIN ){ // on suppose qu'il est impossible de détecter plus de 10 notes pour un clavier seul , cela permet de limiter la boucle quoi qu'il arrive				
				tabchord[i].kech = kmax;
				tabchord[i].note = correspondancenote( kmax , NotesBank );
				
				Z_smoothing( Z, sizeframe , kmax , kmin);
				i++;
			}
			else b = 0;
		}
	}
	free(L);
	return i;
}		


void Hamming( frame x , int sizeframe ){
	int i;
	for(i=0;i<sizeframe;i++){
		x[i] *= 0.53836 - 0.46164*cos(2*M_PI*i/(sizeframe-1)); // on multiplie le signal par une porte de hamming
	}
}


int frameprocessing( chord * tabchord , frame x , int sizeframe , double samplerate, double kmin, double k0 , double k1 , double ** MatrixB, double * b_m0_m2 , double * NoteBank ){
	frame N;
	frame Z;
	double SNR;
	int b=0;
	double *DSP;
	DSP=calloc(sizeframe/2+1,sizeof(double)); // on alloue DSP ici pour le traité dans short_time
	Hamming( x , sizeframe);
	short_time_DSP( x , sizeframe , DSP);
	// DSP est maintenant de taille sizeframe/2+1, on se débarasse de la symétrie du spectre
	if(!DSP ){
		printf("frameprocessing() DSP NULL\n");
		return 0;
	}
	
	N = calloc(sizeframe/2+1, sizeof(double)); // On prépare une frame N contenant la moyenne de Y=ln(1+X/g)
	

	Z  = noisesup( DSP , round(k0) , floor(k1)+1 , sizeframe/2+1 , (double)2./3. , N );
	SNR = SNR_calc( DSP , N , sizeframe/2+1 , k0 , k1); // on calcul une seule fois le rapport signal sur bruit utilisé par la suite

	free(DSP); // une fois que le rapport signal sur bruit (SNR) est calculé et que X est traité , N et X sont inutiles
	

	b = boucle(tabchord,Z,sizeframe/2+1,SNR,kmin,MatrixB,b_m0_m2,NoteBank);
	free(Z);
	if(b>0) return 1; // 0 si rien n'a été modifié dans tabchord
	else return 0; // 1 si on a au moins une note, en sachant que b représente le nombre de notes détectées
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
			if(kmax) *kmax = i; // on peut choisir de mettre kmax=NULL; et dans ce cas la fonction ne renvoie que la valeur maximale
			M = X[i];
		}
	}
	return M;
}


int zerostabofchar( int l ,char* t){
	int i;
	if(!t){
		puts("erreur zeros()");
		return 0;
	}
	else{
		for(i=0;i<l;i++) t[i]=0;
	return 1;
	}
}


Tnote simplifT(Tnote T,int SIZE_T,double tmin){		//Testée
	int i;
	int j;
	int k;
	int l;
	double dT;
	char tnote[SIZE_TABCHORD];
	zerostabofchar(SIZE_TABCHORD,tnote);
	for(i=1;i<SIZE_T;i++){
		j=SIZE_T-i;
		dT = T[j].temps-T[j-1].temps;
		if(dT<=tmin){
			for(k=0;k<SIZE_TABCHORD;k++){
				tnote[k]=T[j].tabchord[k].note;
			}
			for(k=0;k<SIZE_TABCHORD;k++){
				for(l=0;l<SIZE_TABCHORD;l++){
					if(tnote[l]==T[j-1].tabchord[k].note){
						T[j].tabchord[l].note = 0;
						T[j-1].tabchord[k].duree += T[j].tabchord[k].duree;
						T[j].tabchord[l].duree = 0;
					}
				}
			}
		}
	}
	return(T);
}



void mainprocessing( Tnote  T , int sizeTmax , double * datain , int sizedatain  , double samplerate , int sizeframe){
	int j = 0;
	int k = 0;
	int i;
	int b;
	double k0 = (double)FMIN*sizeframe/samplerate;
	double k1 = (double)FMAX*sizeframe/samplerate;
	double kmin = (double)BMIN*sizeframe/samplerate;
	double * NotesBank = creer_notesBank( samplerate , sizeframe );
	double * B_m0_m2 = calloc(BNMAX*2,sizeof(double));
	double ** MatrixB = MatrixBW( sizeframe/2+1, k0 ,  kmin , B_m0_m2);
	double time; // en secondes

	frame x = fftw_malloc(sizeframe*sizeof(double)); // allocation spécifique à la librairie de fftw
	
	if(!x || !datain ){
		puts(" x ou datain NULL");
		return ;
	}

	if( !NotesBank || ! B_m0_m2 || !MatrixB){
		puts("problème allocation NotesBank,B_m0_m2,MatrixB");
		return ;
	}
	
	initTnote( T , sizeTmax , sizeframe , samplerate );

	while(j < sizedatain ){
		for( i=0 ; i < sizeframe  && i+j < sizedatain ; i++ ) x[i] = datain[i+j];
		b = frameprocessing(T[k].tabchord ,  x , sizeframe ,  samplerate,  kmin,  k0 ,  k1 ,   MatrixB,   B_m0_m2 ,  NotesBank);
		if( b == 1 ){ // au moins une note a été détectée donc on stocke le temps de détection et on incrémente k
			time = (double)(j+sizeframe/2)/samplerate; // le temps de détection correspond au centre de la porte
			T[k].temps = time;
			k++;
		}
		j += sizeframe/2; // on avance d'une demie porte
	}
}








