LDIR = lib
SRCDIR = src
IDIR = include

CCFLAGS = -I$(IDIR) -I/usr/include/libxml2

LIBS = -lm -lxml2

_SRCS = driverMain
SRCS = $(patsubst %,$(IDIR)/%.h,$(_SRCS))

_LLIBS = xmlMsgBuffer
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

driverMain: $(MOBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	rm -f *.o

testBin: $(TOBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	mv testBin test
	rm -f *.o
