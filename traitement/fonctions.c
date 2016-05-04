#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "fonctions.h"
#include "textexport.h"
#include "../midi/midi.h"

double * creertab( int n) {
	double * tab;
	double fo = 32.70;
	int i;
	tab=calloc(n,sizeof(*tab));
	for (i=0;i<n;i++){
		tab[i]=fo*pow(2,(double)i/12);}
	return tab;}
  
double incertitude ( int n ) {
	double fo= 32.70;
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
		//printf("%lf\n", Y[i]);
	
	}
	return Y;
	
}

double * moving_average(double * Y, double ratio, int taille){
	double * movingA = calloc(taille, sizeof(*movingA));
	double localA = 0;
	int i,j;
	for(i=0;i<=taille;i++){
		if(i-ratio*i/2 < 0 || i+ratio*i/2 > taille){
			movingA[i] = Y[i];
		}
		else{
			double localA = 0;
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
    for(i=0;i<taille;i++){
        Z[i] = ((Y[i]-N[i])> 0) ? Y[i]-N[i] : 0;
    }
    return Z;
}


void fosup(frame Zsmoothed , frame Z, int taille ){
	int i;
	for(i=0;i<taille;i++){
		Z[i]=Z[i]-Zsmoothed[i] ; }

	}

int processing_init(frame X, frame Lmax,frame Npow, int taille, double threshold,int k0,int k1){
	double Lmax1=0;	
	int i;
	double x=0;
	double N=0;
	double vo=0;
	for(i=0;i<taille;i++){
		Lmax1+=Lmax[i] ;
		}
	for(i=k0;i<k1;i++){
		x+=X[i] ;
		N+=Npow[i];
		}
	vo=4*log(Lmax1)+log(x)-log(N) ;
	if (vo>threshold) return 1;
	if (vo<threshold) return 0;
}

int iteration_checking(frame X, frame Lmaxi,frame Npow, int taille,double threshold, int k0, int k1){
	double Lmax1=0;	
	int i;
	double x=0;
	double N=0;
	double vo=0;
	for(i=0;i<taille;i++){
		Lmax1+=Lmaxi[i] ;
		}
	for(i=k0;i<k1;i++){
		x+=X[i] ;
		N+=Npow[i];
		}
	vo=1.8*log(Lmax1)+log(x)-log(N) ;
	if (vo>threshold) return 1;
	if (vo<threshold) return 0;
	
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
			M=L[i];
			fM = i+1;
		}
	}
	return(fM);
}

double mean(double* T,double m0,double m2){   //Calcul la moyenne entre m0 et m2		//Testée et Approuvée
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


 
double round(double value) {					//Testée et approuvée
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

double* functionBW (double Bmin,int N, double fs, int l, double kb, double ratio, double* M0, double* M2){ // [b,m0,m2]=BW(Bmin,N,fs,l,kb,ratio)  //Testée et approuvée
	int i;
	double* b = zeros(l);
	double kmin = (double)Bmin*N/fs;
	double kb1=round(kb);
	double m1,b1,a1,b2,a2 ;
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
		printf("m1 = %lf",m1);
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







<<<<<<< HEAD
=======

double npow( double* moyennex, int k1,int k0){
	int i;
	double g=0;
	for (i=k0;i<k1;i++){
		g+=pow(moyennex[i],1/3);
	}
	g=g/(k1-k0+1);
	g=pow(g,3);
	double* Npow=zeros(k1-k0+1);
	for(i=k0;i<k1;i++){
		Npow[i]=exp(moyennex[i]-1)*g;
	}
}






>>>>>>> 05aea81147204afbd9702bb960169edcdc2b46c0
