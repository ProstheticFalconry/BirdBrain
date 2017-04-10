CC = g++
CFLAGS = -Wall
LDLIBS+ = -lpthread -lprussdrv

all: althold_test

althold_test:
	@echo "\nCompiling althold test\n"
	$(CC) -c -o althold_test.o althold_test.cpp
	$(CC) -c -o PID.o PID.cpp $(LDLIBS+)
	$(CC) -o althold_test althold_test.o PID.o $(LDLIBS+)

clean:
	rm -rf althold_test althold_test.o
