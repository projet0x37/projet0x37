#include <stdio.h>
#include <stdlib.h>
#include "midi.h"

//int tailleTnote(Tnote tab){

int main(int argc, char** args){
	int t;
	int i;
	Tnote test1;
	Tnote test2;
	Tnote test3;
	Tnote test4;
	test1=calloc(5,sizeof(*test1));
	test2=calloc(5,sizeof(*test1));
	test3=calloc(5,sizeof(*test1));
	test4=calloc(5,sizeof(*test1));
	for(i=0;i<5;i++){ // test OK
		test1[i].temps=(double)i;
		test1[i].tabchord[0].note=60+i;
		test1[i].tabchord[0].duree=(double)1/2;
		test1[i].tabchord[1].note=-1;
		puts("");
		printf("test1.temps: %f, test1noteint: %d %f\n", test1[i].temps, test1[i].tabchord[0].note, test1[i].tabchord[0].duree);
		}

	for(i=0;i<5;i++){ // test OK
		test2[i].temps=(double)i;
		test2[i].tabchord[0].note=60 + i;
		test2[i].tabchord[0].duree=(double)1/2;
		test2[i].tabchord[1].note=70 + i;
		test2[i].tabchord[1].duree=(double)1/2;
		test2[i].tabchord[2].note=-1;
		puts("");
		printf("test2.temps: %f, notes: %d %d durées %f %f\n", test2[i].temps, test2[i].tabchord[0].note , test2[i].tabchord[1].note , test2[i].tabchord[0].duree , test2[i].tabchord[1].duree );
		}

	for(i=0;i<5;i++){ // test OK
		test3[i].temps=(double)i;
		test3[i].tabchord[0].note=60+i;
		test3[i].tabchord[0].duree=(double)3/2;
		test3[i].tabchord[1].note=-1;
		puts("");
		printf("test3.temps: %f, test3noteint: %d %f\n", test3[i].temps, test3[i].tabchord[0].note, test3[i].tabchord[0].duree);
		}

	for(i=0;i<5;i++){ // test OK
		test4[i].temps=(double)i;
		test4[i].tabchord[0].note=60 + i;
		test4[i].tabchord[0].duree=(double)1/2;
		test4[i].tabchord[1].note=70 + i;
		test4[i].tabchord[1].duree=(double)3/2;
		test4[i].tabchord[2].note=-1;
		puts("");
		printf("test4.temps: %f, notes: %d %d durées %f %f\n", test4[i].temps, test4[i].tabchord[0].note , test4[i].tabchord[1].note , test4[i].tabchord[0].duree, test4[i].tabchord[1].duree);
		}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>test noteon noteoff et delay<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/*
	test=fopen("test4","w+b");
	testLi=tabtoliste(test4[0].tabchord);
	puts("testLi");
	for(p=testLi;p;p=p->suiv){
		if(p)printf("%d %f\n",p->note,p->duree);
	}
	noteon(testLi , test );
	testLi=tabtoliste(test4[0].tabchord);
	testLi=tri(&testLi);
	taumax=test4[1].temps - test4[0].temps;
	testSi=split(&testLi, taumax);
	puts("testSi");
	for(p=testSi;p;p=p->suiv){
		if(p)printf("%d %f\n",p->note,p->duree);
	}
	noteoff(testLi,testSi,test);
	fclose(test);
	*/

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>test miditrackdata endmiditrack etc .. relatif aux fichiers<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/*
	test=miditrackdata(test4,5,"testf4");
	

	midiheadf=midihead("midihead");
	miditrackheadf=miditrackhead("trackhead");
	miditrackdataf=miditrackdata(test1,5,"trackdata");
	test=endmiditrack( miditrackheadf ,miditrackdataf , "track" );
	*/
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>test mainmidi, fonction principale<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	puts("entrer le numéro du test entre 1 et 4");
	scanf("%d",&t);
	switch (t)
	{
	case 1:
		mainmidi("outputmidi",test1,5);
		break;
	case 2:
		mainmidi("outputmidi",test2,5);
		break;
	case 3:
		mainmidi("outputmidi",test3,5);
		break;
	case 4:
		mainmidi("outputmidi",test4,5);
		break;
	default:
		mainmidi("outputmidi",test4,5);
		break;
	}
	return EXIT_SUCCESS;
}
