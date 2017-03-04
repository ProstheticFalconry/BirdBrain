LDIR = lib
SRCDIR = src
IDIR = include
CC=gcc
CCFLAGS = -I$(IDIR)

LIBS =

_SRCS = main
SRCS = $(patsubst %,$(IDIR)/%.h,$(_SRCS))

_LLIBS = lib1
LLIBS = $(patsubst %,$(IDIR)/%.h,$(_LLIBS))

OBJ = $(patsubst %,%.o,$(_LLIBS))
MOBJ := $(OBJ) $(patsubst %,%.o,$(_SRCS))
TOBJ := $(OBJ) testBin.o

%.o: $(LDIR)/%.c $(LLIBS) 
	$(CC) -c $(CCFLAGS) $< -o $@

%.o: $(SRCDIR)/%.c $(SRCS)
	$(CC) -c $(CCFLAGS) $< -o $@

testBin.o: test/testBin.c test/testBin.h
	$(CC) -c $(CCFLAGS) $< -o $@

flightControl: $(MOBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	rm -f *.o

testBin: $(TOBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	mv testBin test
	rm -f *.o
