IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

LDIR =../lib

LIBS=-lm

MAIN=driverMain
DEPS = testdep

DEPSH = $(patsubst %,$(IDIR)/%.h,$(DEPS))
DEPSH += $(MAIN).h

DEPSC = $(patsubst %,$(LDIR)/%.c,$(DEPS))
DEPSC += $(MAIN).cpp

OBJ = $(patsubst %,%.o,$(DEPS))
OBJ += $(MAIN).o


$(OBJ).o: $(DEPSC).cpp $(DEPSH).h
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(INCDIR)/*~
