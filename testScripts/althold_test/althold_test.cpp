#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PID.h"

int main() {
	PWM4 * motors = new PWM4();
	
	PRU * sonar = new PRU();

	PID_class * PID = new PID_class(60);
	
	float set_point = 60; // 60 cm target altitude
	float true_altitude = 50; // start at 50 cm

	float duty_cycle = 100;
	motors->set_duty_cycle(duty_cycle);

	clock_t start = clock();
	clock_t difference;
	clock_t loop_time;
	
	while(1) {
		difference = (clock() - start) / CLOCKS_PER_SEC;
		if (((clock() - start) / CLOCKS_PER_SEC) > 15) {
			PID->set_altitude(45.0);
		}
		if (difference > 20) {
			motors->set_duty_cycle(0);
		}

		// PRU stuff
		true_altitude = sonar->get_alt();		


		// PID stuff
		PID->get_error(true_altitude);
		PID->calc_I();
		PID->calc_P();
		PID->calc_D();
		duty_cycle = PID->update_throttle();

		// Motors stuff
		motors->set_duty_cycle(duty_cycle);
		loop_time = clock();
		while (1){
		    // Wait 5 ms
		    if (((clock() - loop_time) / (1000 * CLOCKS_PER_SEC)) > 5) {
			break;
		    }
		}
	}
	return 0;
}
