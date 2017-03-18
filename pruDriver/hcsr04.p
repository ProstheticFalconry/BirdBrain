// Define the entry point of the program
.origin 0
.entrypoint START

// Address of the io controllers for GPIO1, GPIO2 and GPIO3
#define GPIO1 0x4804C000
// #define GPIO2 0x481AC000
// #define GPIO3 0x481AE000

// Address of the PRUn_CTRL registers
#define PRU0_CTRL 0x22000
#define PRU1_CTRL 0x24000

// Offset address for the output enable register of the gpio controller
#define GPIO_OE 0x134

// Offset address for the data in/out register of the gpio controller
#define GPIO_DATAIN 0x138
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194

// Offset address for the CYCLE register of PRU controller
#define CYCLE 0xC

// Bit for enabling the cycle counter on PRU_CTRL register
#define CTR_EN 3

// gpio1[30] P8_21 gpio62 0x80
#define BIT_TRIGGER62 0x1E

// gpio[31] P8_20 gpio63 0x84
#define BIT_ECHO63 0x1F

//gpio1[14] P8_16 gpio46 0x
#define BIT_TRIGGER46 0x0E

//gpio1[15] P8_15 gpio47 0x
#define BIT_ECHO47 0x0F

// gpio1[12] P8_12 gpio44 0x030
#define BIT_TRIGGER44 0x0C

// gpio1[13] P8_11 gpio45 0x034
#define BIT_ECHO45 0x0D

// PRU interrupt for PRU0
#define PRU0_ARM_INTERRUPT 19

// ChooseTriggerAndEchoPins
#define BIT_TRIGGER BIT_TRIGGER46
#define BIT_ECHO BIT_ECHO47
#define BIT_ECHO1 BIT_ECHO45

#define MAX_TIME 2230
#define WAIT_TIME 2000000
#define delay r0
#define roundtrip r4


START:

	// Clear the STANDBY_INIT bit in the SYSCFG register
	// otherwise the PRU will not be able to write outside the PRU memory space
	// and to the Beaglebone pins
	LBCO r0, C4, 4, 4
	CLR r0, r0, 4
	SBCO r0, C4, 4, 4
	
	// Make constant 24 (c24) point to the beginning of PRU0 data ram
        // 0x22000 is PRU_CTRL Registers. 0x20 is the offset for register that determines C24 address
        // SBBO copies 4 bytes of r0 to r1
	MOV r0, 0x00000000
	MOV r1, 0x22020
	SBBO r0, r1, 0, 4

        // Enable Cycle Counter. 0x2200 is address of PRU_CTRL register. CTR_EN is bit the enables the counter
        MOV r1, PRU0_CTRL
        LBBO r0, r1, 0, 4
        SET r0, CTR_EN
        SBBO r0, r1, 0, 4

	// Enable trigger as output and echo as input (clear BIT_TRIGGER and set BIT_ECHO of output enable)
	MOV r1, GPIO1 | GPIO_OE
	LBBO r0, r1, 0, 4
	CLR r0, BIT_TRIGGER
	SET r0, BIT_ECHO
        SET r0, BIT_ECHO1
	SBBO r0, r1, 0, 4

TRIGGER:
        // READ cycle counter
        MOV r1, PRU0_CTRL | CYCLE
        LBBO r2, r1, 0, 4
        SBCO r2, c24, 8, 4

	// Fire the sonar
	// Set trigger pin to high
	MOV r2, 1<<BIT_TRIGGER
	MOV r3, GPIO1 | GPIO_SETDATAOUT
	SBBO r2, r3, 0, 4
	
	// Delay 10 microseconds (200 MHz / 2 instructions = 10 ns per loop, 10 us = 1000 loops) 
	MOV delay, 1000
TRIGGER_DELAY:
	SUB delay, delay, 1
	QBNE TRIGGER_DELAY, delay, 0
	
	// Set trigger pin to low
	MOV r2, 1<<BIT_TRIGGER
	MOV r3, GPIO1 | GPIO_CLEARDATAOUT
	SBBO r2, r3, 0, 4
	
	// Wait for BIT_ECHO to go high, i.e. wait for the echo cycle to start
	MOV r3, GPIO1 | GPIO_DATAIN
        // Initialize timeout
        // MOV r5, MAX_TIME
WAIT_ECHO:
	// Check for TIMEOUT
        // SUB r5, r5, 1
        // QBEQ TIMEOUT, r5, 0
        // Read the GPIO until BIT_ECHO goes high
	LBBO r2, r3, 0, 4
	QBBC WAIT_ECHO, r2, BIT_ECHO

	// roundtrip measures the echo duration in microseconds, resolution is 1us
	MOV roundtrip, 0
SAMPLE_ECHO:

	// Delay 1 microsecond (adjusted because it takes time to query the GPIO pin)
	MOV delay, 76
SAMPLE_ECHO_DELAY:
	SUB delay, delay, 1
	QBNE SAMPLE_ECHO_DELAY, delay, 0
	
	// Add 1us to the roundtrip counter
	ADD roundtrip, roundtrip, 1

        // Read GPIO until BIT_ECHO goes low
	LBBO r2, r3, 0, 4
	QBBS SAMPLE_ECHO, r2, BIT_ECHO
        QBA NO_TIMEOUT
TIMEOUT:
        MOV roundtrip, 0xFFFFFFFF
NO_TIMEOUT:
	// Echo is complete
	// Store the microsecond count in the PRUs data ram so C program can read it
	SBCO roundtrip, c24, 0, 4

        // Interrupt to cause printf to trigger
        // MOV r31.b0, PRU0_ARM_INTERRUPT+16
        
        // Delay to allow sonar to stop resonating and sound burst to decay in environment
        MOV delay, WAIT_TIME
RESET_DELAY1:
        SUB delay, delay, 1
        QBNE RESET_DELAY1, delay, 0


        // Fire the sonar
        // Set trigger pin to high
        MOV r2, 1<<BIT_TRIGGER
        MOV r3, GPIO1 | GPIO_SETDATAOUT
        SBBO r2, r3, 0, 4
        
        // Delay 10 microseconds (200 MHz / 2 instructions = 10 ns per loop, 10 us = 1000 loops)
        MOV delay, 1000
TRIGGER_DELAY1:
        SUB delay, delay, 1
        QBNE TRIGGER_DELAY1, delay, 0
        
        // Set trigger pin to low
        MOV r2, 1<<BIT_TRIGGER
        MOV r3, GPIO1 | GPIO_CLEARDATAOUT
        SBBO r2, r3, 0, 4
        
        // Wait for BIT_ECHO1 to go high, i.e. wait for the echo cycle to start
        MOV r3, GPIO1 | GPIO_DATAIN
        // MOV r5, MAX_TIME
WAIT_ECHO1:
        // Check for timeout
        // SUB r5, r5, 1
        // QBEQ TIMEOUT1, r5, 0

        // Read the GPIO until BIT_ECHO1 goes high
        LBBO r2, r3, 0, 4
        QBBC WAIT_ECHO1, r2, BIT_ECHO1

        // roundtrip measures the echo duration in microseconds, resolution is 1us
        MOV roundtrip, 0

SAMPLE_ECHO1:
        // Delay 1 microsecond (adjusted because it takes time to query the GPIO pin)
        MOV delay, 76
SAMPLE_ECHO_DELAY1:
        SUB delay, delay, 1
        QBNE SAMPLE_ECHO_DELAY1, delay, 0
        
        // Add 1us to the roundtrip counter
        ADD roundtrip, roundtrip, 1
        
        // Read GPIO until BIT_ECHO1 goes low
        LBBO r2, r3, 0, 4
        QBBS SAMPLE_ECHO1, r2, BIT_ECHO1
        QBA NO_TIMEOUT1
TIMEOUT1:
        MOV roundtrip, 0xFFFFFFFF
NO_TIMEOUT1:

        // Echo is complete
        // Store the microsecond count in the PRUs data ram so C program can read it
        // SBCO roundtrip, c24, 32, 4        
        SBCO roundtrip, c24, 4, 4

	//MOV r31.b0, PRU0_ARM_INTERRUPT+16
	
	// Delay to allow sonar to stop resonating and sound burst to decay in environment
	MOV delay, WAIT_TIME
RESET_DELAY:
	SUB delay, delay, 1
	QBNE RESET_DELAY, delay, 0
	
	// Jump back to triggering the sonar
	JMP TRIGGER
	
	HALT

