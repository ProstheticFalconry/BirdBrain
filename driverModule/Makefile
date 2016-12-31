CXXFLAGS=-I$(IDIR)

LDIR=lib
SRCDIR=src
IDIR=include


LIBS=-lm

_SRCS=driverMain
SRCS=$(patsubst %,$(IDIR)/%.h,$(_SRCS))

_LLIBS = testdep
LLIBS=$(patsubst %,$(IDIR)/%.h,$(_LLIBS))

OBJ = $(patsubst %,%.o,$(_LLIBS))
OBJ += $(patsubst %,%.o,$(_SRCS))


%.o: $(LDIR)/%.cpp $(LLIBS) 
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.o: $(SRCDIR)/%.cpp $(SRCS)
	$(CXX) -c $(CXXFLAGS) $< -o $@

hellomake: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(INCDIR)/*~
