
#gcc -c audioprocess.c -I .
#gcc -c textexport.c
#gcc *.o -L. -lsndfile -lm

CC=gcc
CFLAGS= -c -g
LFLAGS = -L. -lsndfile -lm
all: audio.exe

audio.exe : textexport.o audioprocess.o testmain.o
	$(CC) *.o $(LFLAGS) -o audio.exe

*.o: *.c
	$(CC) $(CFLAGS) *.c 


clean:
	rm *.o audio.exe
