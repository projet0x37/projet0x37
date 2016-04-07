#include <stdio.h>
#include <stdlib.h>
#include "midi.h"

//int tailleTnote(Tnote tab){


void main(void){
	FILE * test;
	liste testLi;
	int i;
	Tnote test1;
	Tnote test2;
	Tnote test3;
	Tnote test4;
	test1=calloc(5,sizeof(*test1));
	test2=calloc(5,sizeof(*test1));
	test3=calloc(5,sizeof(*test1));
	test4=calloc(5,sizeof(*test1));
	for(i=0;i<5;i++){
		test1[i].temps=(double)i;
		test1[i].tabchord[0].note=60+i;
		test1[i].tabchord[0].duree=(double)1/2;
		test1[i].tabchord[1].note=-1;
		puts("");
		printf("test1.temps: %f, test1noteint: %d %f\n", test1[i].temps, test1[i].tabchord[0].note, test1[i].tabchord[0].duree);
		}

	for(i=0;i<5;i++){
		test2[i].temps=(double)i;
		test2[i].tabchord[0].note=60 + i;
		test2[i].tabchord[0].duree=(double)1/2;
		test2[i].tabchord[1].note=70 + i;
		test2[i].tabchord[1].duree=(double)1/2;
		test2[i].tabchord[2].note=-1;
		puts("");
		printf("test2.temps: %f, notes: %d %d durées %f %f\n", test2[i].temps, test2[i].tabchord[0].note , test2[i].tabchord[1].note , test2[i].tabchord[0].duree , test2[i].tabchord[1].duree );
		}

	for(i=0;i<5;i++){
		
		test3[i].temps=(double)i;
		test3[i].tabchord[0].note=60+i;
		test3[i].tabchord[0].duree=(double)3/2;
		test3[i].tabchord[1].note=-1;
		puts("");
		printf("test3.temps: %f, test3noteint: %d %f\n", test3[i].temps, test3[i].tabchord[0].note, test3[i].tabchord[0].duree);
		}

	for(i=0;i<5;i++){
		test4[i].temps=(double)i;
		test4[i].tabchord[0].note=60 + i;
		test4[i].tabchord[0].duree=(double)1/2;
		test4[i].tabchord[1].note=70 + i;
		test4[i].tabchord[1].duree=(double)3/2;
		test4[i].tabchord[2].note=-1;
		puts("");
		printf("test4.temps: %f, notes: %d %d durées %f %f\n", test4[i].temps, test4[i].tabchord[0].note , test4[i].tabchord[1].note , test4[i].tabchord[0].duree, test4[i].tabchord[1].duree);
		}
	test=fopen("test1","w+b");
	testLi=tabtoliste(test2[0].tabchord);
	printf("%p %d \n ", testLi,testLi->note);
	noteon(testLi , test );
	fclose(test);
	
		
}
