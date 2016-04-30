
CC=gcc

CFLAGS=-c -Wall -g

OBJECTS = listemidi.o midi.o testmidi.o

all: midi.exe

midi.exe: $(OBJECTS)
	$(CC) $(OBJECTS) -lm -o midi.exe

listemidi.o: listemidi.c
	$(CC) $(CFLAGS) listemidi.c

midi.o: midi.c
	$(CC) $(CFLAGS) midi.c

testmidi.o: testmidi.c
	$(CC) $(CFLAGS) testmidi.c

clean:
	rm -f $(OBJECTS) midi.exe

