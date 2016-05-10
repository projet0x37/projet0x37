#include <stdio.h>
#include <stdlib.h>
#include "textexport.h"


void textexport(char * textname,double * data,int taille, int taille_utilisateur){
	FILE * f1;
	int i;
	f1=fopen(textname,"w");
	if(data){
		for(i=0;i<taille && i < taille_utilisateur;i++){
			fprintf(f1,"%lf\n",data[i]);
		}
	}
	fclose(f1);
}


void textexport_Mat(char * textname , double ** data , int colonne, int ligne){
	FILE * f1;
	int i,j;
	f1=fopen(textname,"w");
	if(data){
		for(j=0;j<ligne;j++){
			for(i=0;i<colonne;i++){
				if(i==colonne-1)fprintf(f1,"%lf\n",data[j][i]);
				else fprintf(f1,"%lf ",data[j][i]);
			}
		}
	}
	fclose(f1);
}


double* tabexportfromtext(char * textname,int nb){
	double * t = calloc(nb,sizeof(*t));	
	int i=0;
	double a;
	int taille=0;
	FILE * ft = fopen(textname,"r");
	while(fscanf(ft,"%lf",&a)!=0 && i < nb){
		t[i]=a;
		taille++;
		i++;
	}	
	fclose(ft);
	return t;
}	
