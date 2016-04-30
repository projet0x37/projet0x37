#include <stdio.h>
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
	
	
