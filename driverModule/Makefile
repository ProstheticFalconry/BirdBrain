LDIR = lib
SRCDIR = src
CC=gcc
#CCFLAGS = -Ilib -I/lib/modules/4.4.47-bone-rt-r16/build/arch/arm/include/

_SRCS = driverMain
SRCS = $(patsubst %,%.h,$(_SRCS))

_LLIBS = ringBuf
LLIBS = $(patsubst %,lib/%.h,$(_LLIBS))

OBJ = $(patsubst %,%.o,$(_LLIBS))
MOBJ := $(OBJ) $(patsubst %,%.o,$(_SRCS))
TOBJ := $(OBJ) testBin.o
obj-m += flightControl.o
flightControl-objs := lib/ringBuf.o flightControlMain.o


%.o: $(LDIR)/%.c $(LLIBS) 
	$(CC) -c $(CCFLAGS) $< -o $@

%.o: $(SRCDIR)/%.c $(SRCS)
	$(CC) -c $(CCFLAGS) $< -o $@

testBin.o: test/testBin.c test/testBin.h
	$(CC) -c $(CCFLAGS) $< -o $@

flightControl:
	make -C /lib/modules/$(shell uname -r)/build M=/home/ubuntu/BirdBrain/driverModule modules
	rm -f *.o

testBin: $(TOBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)
	mv testBin test
	rm -f *.o
