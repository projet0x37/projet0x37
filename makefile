CC=gcc
CFLAGS= -c -g -Wall
LFLAGS = -L audio/. audio/libsndfile.a -lm -lfftw3

OBJECTS = audio/audioprocess.o main.o midi/listemidi.o midi/midi.o traitement/fonctions.o textexport/textexport.o

all: main.exe

main.exe : $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o main.exe

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) main.exe
