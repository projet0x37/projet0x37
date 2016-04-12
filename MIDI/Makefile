
CC=gcc

CFLAGS=-c -Wall -g

all: midi.exe

midi.exe: listemidi.o midi.o testmidi.o
	$(CC) listemidi.o midi.o testmidi.o -lm -o midi.exe

listemidi.o: listemidi.c
	$(CC) $(CFLAGS) listemidi.c

midi.o: midi.c
	$(CC) $(CFLAGS) midi.c

testmidi.o: testmidi.c
	$(CC) $(CFLAGS) testmidi.c

clean:
	rm *.o midi.exe

