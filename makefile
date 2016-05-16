CC=gcc
CFLAGS=  -c -g -Wall
LFLAGS =  -L audio/. audio/libsndfile.a -lm -L traitement/ traitement/libfftw3.a

OBJECTS = audio/audioprocess.o main.o midi/listemidi.o midi/midi.o traitement/fonctions.o textexport/textexport.o

all: projet0x37.exe

projet0x37.exe : $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o projet0x37.exe

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) projet0x37.exe
