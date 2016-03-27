#include <stdio.h>
#include <stdlib.h>
#include "midi.h"

//int tailleTnote(Tnote tab){


void main(void){
	int i;
	Tnote test1;
	Tnote test2;
	Tnote test3;
	Tnote test4;
	polynote* noteint=NULL;
	test1=calloc(5,sizeof(*test1));
	test2=calloc(5,sizeof(*test1));
	test3=calloc(5,sizeof(*test1));
	test4=calloc(5,sizeof(*test1));	
	for(i=0;i<5;i++){
		noteint=calloc(1,sizeof(*noteint));
		noteint->note=60+i;
		noteint->duree=(double)1/2;
		test1[i].temps=(double)i;
		test1[i].tabnote=noteint;
		puts("");
		//printf("noteint->note: %d, noteint->durée: %f\n", noteint->note, noteint->duree);
		printf("test1.temps: %f, test1noteint: %d %f\n", test1[i].temps, test1[i].tabnote->note, test1[i].tabnote->duree);
		free(noteint);}

	for(i=0;i<5;i++){
		noteint=calloc(2,sizeof(*noteint));
		noteint[0].note=60+i;
		noteint[1].note=70+i;
		noteint[0].duree=(double)1/2;
		noteint[1].duree=(double)1/2;
		test2[i].temps=(double)i;
		test2[i].tabnote=noteint;
		puts("");
		//printf("noteint->note: %d %d, noteint->durée: %f %f\n", noteint->note , (noteint+1)->note, noteint->duree ,(noteint+1)->duree);
		printf("test2.temps: %f, notes: %d %d durées %f %f\n", test2[i].temps, test2[i].tabnote->note, test2[i].tabnote->duree , test2[i].tabnote[1].note, test2[i].tabnote[1].duree);
		free(noteint);}
	for(i=0;i<5;i++){
		noteint=calloc(1,sizeof(*noteint));
		noteint->note=60+i;
		noteint->duree=(double)3/2;
		test3[i].temps=(double)i;
		test3[i].tabnote=noteint;
		puts("");
		//printf("noteint->note: %d, noteint->durée: %f\n", noteint->note, noteint->duree);
		printf("test3.temps: %f, test3noteint: %d %f\n", test3[i].temps, test3[i].tabnote->note, test3[i].tabnote->duree);
		free(noteint);}
	for(i=0;i<5;i++){
		noteint=calloc(2,sizeof(*noteint));
		noteint[0].note=60+i;
		noteint[1].note=70+i;
		noteint[0].duree=(double)1/2;
		noteint[1].duree=(double)3/2;
		test4[i].temps=(double)i;
		test4[i].tabnote=noteint;
		puts("");
		///printf("noteint->note: %d, noteint->durée: %f\n", noteint->note , (noteint+1)->note, noteint->duree ,(noteint+1)->duree);
		printf("test4.temps: %f, notes: %d %d durées %f %f\n", test4[i].temps, test4[i].tabnote->note, test4[i].tabnote->duree , test4[i].tabnote[1].note, test4[i].tabnote[1].duree);
		free(noteint);}
		
}
