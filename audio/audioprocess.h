#include <stdio.h>
#include "sndfile.h"


double * channelread(int nbchannels, double *data ,int frames,int channel);
double * mainaudio(char * infilename,int * taille);


