#include "PID.h"
#include <pruss/prussdrv.h>
#include <pruss/pruss_intc_mapping.h>
#include <stdlib.h>
#include <stdio.h>

PWM4::PWM4() {
	fd1.open(MOT1);
	fd2.open(MOT2);
	fd3.open(MOT3);
        fd4.open(MOT4);
}

void PWM4::set_duty_cycle(float throttle) {
	duty_cycle = (int) ((throttle + 860) * 1000);
	sprintf(buff, "%d", duty_cycle);
	fd1 << buff;
	fd2 << buff;
	fd3 << buff;
	fd4 << buff;
}


PID_class::PID_class(float alt) {
	target_alt = alt;
	error_sum = 0;
	error = 0;
	last_error = 0;
	d_error = 0;
	delta_throttle = 0;
	throttle = 860;
}
void PID_class::set_altitude(float alt) {
	target_alt = alt;
}

void PID_class::get_error(float true_alt) {
	error = true_alt - target_alt;
}

void PID_class::calc_P() {
	delta_throttle = delta_throttle - k_P * error;
}

void PID_class::calc_I() {
	error_sum = error_sum + error;
	if (error_sum < -I_max) {
		error_sum = -I_max;
	} else if (error_sum > I_max) {
		error_sum = I_max;
	}
	delta_throttle = delta_throttle - k_I * error_sum;
}

void PID_class::calc_D() {
	d_error = (error - last_error) / (time_step / ms_per_sec);
	delta_throttle = delta_throttle - k_D * d_error;
}


float PID_class::update_throttle(){
	throttle = throttle + delta_throttle;
	delta_throttle = 0;
	return throttle;
}

/****************************************************
PRU STUFF
****************************************************/

PRU::PRU() {
	if (prussdrv_open (PRU_EVTOUT_0)) {
                // Handle failure
                printf(">> PRU open failed\n");
                return;
        }
	
	//prussdrv_pruintc_init(&pruss_intc_initdata);
	void *pruDataMem;
        prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
        pruData = (unsigned int *) pruDataMem;
	pruData[0] = 50;
	pruData[1] = 50;
}

float PRU::get_alt() {
	altitude = (float) (pruData[1]- pruData[0]) * SPEED_OF_SOUND / (2 * CYCLES_PER_SEC);
	return altitude;
}



