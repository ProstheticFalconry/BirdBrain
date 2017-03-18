#include <stdio.h>
#include <unistd.h>

#include <pruss/prussdrv.h>
#include <pruss/pruss_intc_mapping.h>

#define CYCLES_PER_SEC 200000000
#define SPEED_OF_SOUND 34000 // in cm/s

int main(void) {

	/* Initialize the PRU */
	printf(">> Initializing PRU\n");
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_init();

	/* Open PRU Interrupt */
	if (prussdrv_open (PRU_EVTOUT_0)) {
		// Handle failure
		fprintf(stderr, ">> PRU open failed\n");
		return 1;
	}
	/* Get the interrupt initialized */
	//prussdrv_pruintc_init(&pruss_intc_initdata);
	
	/* Get pointers to PRU local memory */
	void *pruDataMem;
	prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
	unsigned int *pruData = (unsigned int *) pruDataMem;

	/* Initialize Variables */
	double range_0 = 0;
        double range_1 = 0;
        pruData[0] = 0;
        pruData[1] = 0;
        pruData[2] = 0;
        pruData[3] = 0;
        pruData[4] = 0;
        pruData[5] = 0;
        pruData[6] = 0;
        pruData[7] = 0;


	/* Execute code on PRU */
	printf(">> Executing HCSR-04 code\n");
	prussdrv_exec_program(0, "hcsr04.bin");
	/* Get measurements */
	int i = 0;
	
	while (i++ < 20) {
		printf("\nLoop Number: %u\n", pruData[4]);		
		//for (int j = 0; j < 1000000; j++);
		// Wait for the PRU interrupt
		//prussdrv_pru_wait_event (PRU_EVTOUT_0);
		//prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
		
		// Print the distance received from the sonar
		// At 20 degrees in dry air the speed of sound is 342.2 cm/sec
		// so it takes 29.12 us to make 1 cm, i.e. 58.44 us for a roundtrip of 1 cm
		
		//printf("Sonar 1: (%3d) Distance = %.2f cm\n", i, (float) pruData[0] / 58.44);
		//printf("Sonar 2: (%3d) Distance = %.2f cm\n", i, (float) *(pruData+1) / 58.44);
		range_0 = (double) (pruData[1] - pruData[0]) * SPEED_OF_SOUND / (2 * CYCLES_PER_SEC);
		range_1 = (double) (pruData[3] - pruData[2]) * SPEED_OF_SOUND / (2 * CYCLES_PER_SEC);	
		
		printf("(%2d) Sonar 1: Distance = %.2lf cm\n", i, range_0);
		printf("     Sonar 2: Distance = %.2lf cm\n", range_1);
		sleep(1);
	}

	/* Disable PRU and close memory mapping*/
	prussdrv_pru_disable(0);
	prussdrv_exit();
	printf(">> PRU Disabled.\r\n");
	
	return (0);

}
