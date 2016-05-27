#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "audio/audioprocess.h"
#include "traitement/fonctions.h"
#include "midi/midi.h"
#include "textexport/textexport.h"
#include "traitement/fftw3.h"


double facteurmoyenne = 1;
double thresv0 = THRESV0;
double thresvi = THRESVI;
FILE * logfile;
int boollog = 0;
int userchannel = 0;
int16_t divnoire = 240;


void disphelp(void){
	printf("\nAide Projet0x37\nUsage : ./projet0x37 [options] [fichier]\n\nProjet0x37 fourni la partition MIDI d'un fichier audio .wav .\n\nPour plus d'informations : projet0x37.wordpress.com  ou  github.com/projet0x37 .\n\n");
	printf("Description des options :\n\n");
	printf("  -c X			choisit le canal X du fichier audio.\n");
	printf("			     Exemple pour un stéréo : X = 0 pour le canal de gauche, X = 2 pour le canal de droite.\n");
	printf("			     X prend des valeurs entre 0 et ( nombre de canal - 1 )\n\n");
	printf("  -f fichier  		spécifie le nom du fichier .wav à traiter, sans cette option le programme traite automatiquement le fichier intitulé input.wav dans le répertoire d'exécution.\n\n");
	printf("  -h			affiche cette aide.\n\n");
	printf("  -l			fourni un fichier log récapitulant le traitement du fichier.\n\n");
	printf("  -m FA			avec cette option, la moyenne glissante calculée lors du traitement du bruit est multipliée par FA\n");
	printf("			     cela permet parfois d'améliorer les partitions obtenues, ce facteur est fixé à 1 par défaut\n");
	printf("			     plus il est important et plus le spectre est filtré et inversement, on le modifie aux alentours de 1 et il doit être positif.\n\n");
	printf("  -o thresv0		modifie la valeur de thresv0, elle est de 5 par défaut. Il est conseillé d'utiliser cette option pour obtenir la meilleur partition possible\n");
	printf("			     elle conditionne la détection de la première note à un instant donné et est prioritaire sur la détection des notes suivantes ( voir -w )\n");
	printf("			     si elle est trop importante le logiciel détectera moins de notes, si elle est trop faible il détectera des notes 'fantomes'\n\n");
	printf("  -p thresvI		modifie la valeur de thresvi, elle est de 11 par défaut. Il est conseillé d'utiliser cette option pour obtenir la meilleur partition possible\n");
	printf("			     elle permet de modifier la sensibilité du programme à la polyphonie de l'enregistrement\n");
	printf("			     cette valeur est inversement proportionnelle au nombre de notes détectées à un instant donné ( en excluant la première note, voir -v).\n\n");
	printf("  -t N			modifie la taille de la porte utilisée pour le traitement, N prend des valeurs entre 1 et 4 inclus.\n");
	printf("			     1 : 2048    2 : 4096    3 : 8192    4 : 16384    Par défaut N = 3\n\n");
	printf("  -D division		modifie la valeur de la division de la noire");
}

int main(int argc, char** argv){
	double * datain=NULL;
	int  size;
	double duration;
	double samplerate;
	int sizeframe = 4096 ;
	int sizeTmax;
	int b = 3;
	int c;
	char *filename=NULL;
	char *logfilename=NULL;
	char * midiname =NULL;
	Tnote T;
	userchannel = 0;
	divnoire = 240;
	opterr = 0;
	
	while((c=getopt(argc,argv,"D:c:o:t:p:m:f:lh")) != -1){
		switch (c)
		{
			case 'o' :
				thresv0 = (double)atof(optarg);
				break;
			case 'c' :
				userchannel = (int)atof(optarg);
				break;
			case 'p' :
				thresvi = (double)atof(optarg);
				break;
			case 'm' : 
				facteurmoyenne = (double)atof(optarg);
				break;
			case 'l' :
				boollog = 1;
				break;
			case 't' :
				b = (int)atof(optarg);
				break;
			case 'D' :
				divnoire = (int)atof(optarg);
				break;
			case 'h' :
				disphelp();
				return 0;
				break;
			case 'f' :
				filename=calloc(strlen(optarg)+1,sizeof(char));
				strcpy(filename,optarg);
				//printf("Importation du fichier audio %s ...\n",optarg);
				//datain=mainaudio(optarg,&size,&samplerate);
				break;
			case '?' :
				if(optopt == 'o' || optopt == 'p' || optopt == 'm' || optopt == 't' || optopt =='D' || optopt =='c' || optopt =='f') fprintf (stderr, "Option -%c nécessite un argument.\n", optopt);
				else if ( isprint(optopt) ) fprintf (stderr, "Option inconnue `-%c.\n", optopt);
				else fprintf(stderr,"Caractère d'option inconnue`\\x%x'.\n",optopt);
				return 1;
			default:
				boollog=0;
				abort();
		}
	}

	if(!datain){
		if(!filename){
			printf("Importation du fichier audio input.wav ...\n");
			datain=mainaudio("input.wav",&size,&samplerate);
		}
		else {
			printf("Importation du fichier audio %s ...\n",filename);
			datain=mainaudio(filename,&size,&samplerate);
		}
	}
	
	if(!datain){
		disphelp();		
		return 0;
	}
	
	duration=(double)size/samplerate;
	switch (b)
	{
	case 1 : 
		sizeframe = 2048;
		break;
	case 2 : 
		sizeframe = 4096;
		break;
	case 3 : 
		sizeframe = 8192;
		break;
	case 4 : 
		sizeframe = 16384;
		break;
	default :
		sizeframe = 4096;
		break;
	}

	
	if(boollog){
		if(!filename)logfile=fopen("log-projet0x37-input.wav","w");
		else{
			logfilename=calloc(100,sizeof(char));
			sprintf(logfilename,"log-projet0x37-%s",filename);
			logfile=fopen(logfilename,"w");
		}
		fprintf(logfile,"Log projet0x37.exe\nFréquence d'échantillonnage : %lf Canal : %d Durée : %lf\nLargeur des portes : %lf Résolution : %lf\nThresv0 : %lf Thresvi : %lf\nVaration minimale de vi : %lf Nombre d'itération maximale : %d\n",samplerate,userchannel,duration,sizeframe/samplerate,sizeframe/(2*samplerate),thresv0,thresvi,DELTAMIN,IMAX);
	}

	if( facteurmoyenne < 0 ) facteurmoyenne =1;
	sizeTmax = 2*size/sizeframe + 2;

	T=calloc(sizeTmax,sizeof(*T));
	printf("Traitement du fichier... \n");
	mainprocessing( T ,  sizeTmax , datain , size  , samplerate , sizeframe);
	if(boollog)fclose(logfile);

	//mainmidi("outputmidi.mid",T,sizeTmax);
	T=simplifT(T,sizeTmax);
	//T=condT(T,sizeTmax,0.101); prototype
	if(!filename){
		printf("Création du fichier midi outputmidi-input.wav.mid ...\n");
		mainmidi("outputmidi-input.wav.mid",T,sizeTmax);
	}
	else{
		midiname=calloc(100,sizeof(char));
		sprintf(midiname,"outputmidi-%s.mid",filename);
		printf("Création du fichier midi %s ...\n",midiname);
		mainmidi(midiname,T,sizeTmax);
	}
	printf("Succès ! \n");
	if(boollog){
		if(!filename)printf("Les informations relatives au traitement du fichier sont disponibles dans log-projet0x37-input.wav\n");
		else printf("Les informations relatives au traitement du fichier sont disponibles dans %s\n",logfilename);
	}
	return 0;
}

