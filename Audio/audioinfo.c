#include <stdio.h>
#include <string.h>
#include <sndfile.h>

int main(void){
SNDFILE* infile;
SF_INFO sfinfo;
char *infilename="test.wav";
if (! (infile = sf_open (infilename, SFM_READ, &sfinfo)))/*condition prise dans sfprocess.c dispo dans les exemples de la biblio sndfile */
	{
		printf ("Not able to open input file %s.\n", infilename) ;
		puts (sf_strerror (NULL)) ;
		return 1 ;
		} ;
printf("samples:%ld, samplerate:%d, channels:%d, format:0x%x\n",sfinfo.frames,sfinfo.samplerate,sfinfo.channels,sfinfo.format);
sf_close(infile);
return 0;
}
