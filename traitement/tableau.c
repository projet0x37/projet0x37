#include <stdio.h>
#include <stdlib.h>
#include <math.h>





float* creertab( int n) { float* tab;
    float fo = 32.70;
    int i;
    tab=calloc(n,sizeof(*tab));
    for (i=0;i<n;i++){
        tab[i]=fo*pow(2,(float)i/12);}
    return tab;}
  
float incertitude ( int n ) {
	float fo= 32.70; float i ;
	i= (fo*pow(2,(float)n/12)*(pow(2,(float)1/12)-1))/2 ;
	return i ;
    }

    


char correspondancenote(float fech,int n ){
    int j=0;
	char note;
    float* tab=creertab(n) ;
    for(j=0;j<n;j++) {
        if ( abs(tab[j]-fech)< incertitude(j) ) {
            note = j ; } }
    return note;} 
main(){
	float* testT;
	float* testt=creertab(96);
	float fech;
	char note;
	testT=calloc(1,sizeof(*testT));
	scanf("%f",&fech);
	note=tableau(fech,96);
	printf("%d %f \n",note,testt[note]);}
