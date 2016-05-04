//
//  fonctions.c
//  A man named Klapury
//
//  Created by Vincent Maladiere on 15/04/16.
//  Copyright © 2016 Vincent Maladiere. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fonctions.h"
#include <stdlib.h>
#include <time.h>
#include "textexport.h"

double frand_a_b(double a, double b){
    return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

double * Y_extraction(double * X, int taille){
    int k0 = 0, k1 = 1000; int l = k0;
    double g, somme = 0;
    int i;
    while(l<k1){
        somme += pow(X[l],1/3);
        l += 1;                            //On prend arbitrairement un pas de 1Hz
    }
    somme = somme/(k1-k0+1);
    g = pow(somme,3);
    printf("g = %lf\n",g);
    double * Y = NULL;
    Y = calloc(taille, sizeof(Y));
    for(i=k0;i<k1;i++){
        *(Y+i-k0) = log(1 + 1/g*(*(X+i-k0)));
        printf("%lf\n", *(Y+i-k0));
    }
    return Y;
    }

double * moving_average(double * Y, double largeur, int taille){
    double * movingA = calloc(taille, sizeof(*movingA));
    int i,j;
    for(i=0;i<=taille;i++){
        if(i-largeur/2 < 0 || i+largeur/2 > taille){
            movingA[i] = Y[i];
        }
        else{
            double localA = 0;
            for(j=i-largeur/2;j<=i+largeur/2;j++){
                localA += Y[j];
            }
            movingA[i] = localA/largeur;
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
    for(i=0;i<=80;i++){
        *(Z+i) = 0;
    }
    for(i=930;i<=1000;i++){
        *(Z+i) = 0;
    }
    return Z;
}


void fosup(frame Zsmoothed, frame Z, int taille ){
	int i;
	for(i=0;i<taille;i++){
		Z[i]=Z[i]-Zsmoothed[i] ;
	}
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
	vo=4*ln(Lmax1)+ln(x)-ln(N) ;
	if (vo>threshold) return 1;
	if (vo<threshold) return 0;
	}

int iteration_checking(frame X, frame Lmaxi,frame Npow, int taille,double treshold, int k0, int k1){
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
	vo=1.8*ln(Lmax1)+ln(x)-ln(N) ;
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





