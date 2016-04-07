#ifndef MIDI_H
#define MIDI_H

#include <stdio.h>
#include <stdlib.h>
#include "listemidi.h"
/*swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000); // byte 0 to byte 3*/


typedef struct{
	char note;
	double duree;
	} chord;

typedef struct{
	chord tabchord[31]; //accord dont les notes sont associées à des durées , un tableau de notes avec leur durée
	double temps;
	}notes, *Tnote;


FILE * midihead( char * partition ); // initiliase le fichier midi avec l'entéte principal

FILE * miditrackhead( char * trackhead ); // écrit un fichier contenant l'entéte d'un track sans la partie length

FILE * miditrackdata( Tnote tab , int tailletab , char * trackdata );  // écrit un fichier contenant les données , on inclut les octets pour cloturer le track

long int tailletrack( FILE * miditrackdata ); // renvoit la taille en octets des données écrites dans miditrackdata

FILE * endmiditrack( FILE * miditrackhead , FILE * miditrackdata , char * track ); // finalise l'entéte du track en écrivant la taille des données (length) et fusionne les

FILE * endmidi( FILE * midihead , FILE * miditrack , char * midiname ); // finalise le fichier midi

void noteon( liste Li , FILE * midibin );// fonction qui écrit le mode action consacré aux appels des notes (noteon 90 )

void noteoff( liste Li , liste Si , FILE * miditrackdata ); // fonction qui écrit le mode action consacré a l'extinction des notes (noteoff 80)

void delay( liste Si , double taumax , FILE * miditrackdata ); // fonction qui compléte les délais entre un noteoff et un noteon 

bufferc convert_128( double tau );// permet de convertir un temps(s)  en nombre de division de la noire , les octets sont renvoyés sous forme de liste et elle est utilisée par delay()

liste split( liste Li, double taumax);// sépare en deux la liste de notes elle renvoit NULL si la durée de ttes les notes sont inférieures a tip - ti, une liste sinon

void ajustement( liste Li, double tau );// a mesure qu'on écrit les délais dans le fichier , on ajuste les durées de toutes les notes des listes.

FILE * mergeandclose( FILE * f1 , FILE * f2 ,char * filename); //permet de fusionner deux fichiers

FILE * endmidi( FILE * midihead , FILE * miditrack , char * midiname );// finalise le fichier midi

void mainmidi( char * midi , Tnote tab , int tailletab ); // fonction principale , l'écriture du midi se résume a elle seule , elle nécessite le nom du fichier midi désiré, le tableau de note et sa taille , on peut éventuellement ajouter en paramétre d'autres informations comme la division de la noire , le nombre de track etc ...

#endif
