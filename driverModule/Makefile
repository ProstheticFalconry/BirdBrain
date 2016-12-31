

LDIR=lib
SRCDIR=src
IDIR=include

CCFLAGS=-I$(IDIR)

LIBS=-lm

_SRCS=driverMain
SRCS=$(patsubst %,$(IDIR)/%.h,$(_SRCS))

_LLIBS = testdep
LLIBS=$(patsubst %,$(IDIR)/%.h,$(_LLIBS))

OBJ = $(patsubst %,%.o,$(_LLIBS))
OBJ += $(patsubst %,%.o,$(_SRCS))


%.o: $(LDIR)/%.c $(LLIBS) 
	$(CC) -c $(CCFLAGS) $< -o $@

%.o: $(SRCDIR)/%.c $(SRCS)
	$(CC) -c $(CCFLAGS) $< -o $@

driverMain: $(OBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	rm -f *.o *~ core $(INCDIR)/*

testBin: $(OBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	mv testBin test
	rm -f *.o *~ core $(INCDIR)/*
