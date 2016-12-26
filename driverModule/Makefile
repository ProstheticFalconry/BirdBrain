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
DEPSC += $(MAIN).c

OBJ = $(patsubst %,%.o,$(DEPS))
OBJ += $(MAIN).o


%.o: %.c $(DEPSC) %.h $(DEPSH)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(INCDIR)/*~ 
