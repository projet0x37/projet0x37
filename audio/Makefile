
#gcc -c audioprocess.c -I .
#gcc -c textexport.c
#gcc *.o -L. -lsndfile -lm

CC=gcc
CFLAGS= -c -g -Wall
LFLAGS = -L. libsndfile.a -lm

OBJECTS = textexport.o audioprocess.o testmain.o

all: audio.exe

audio.exe : $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o audio.exe

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@ 


clean:
	rm -f $(OBJECTS) audio.exe
