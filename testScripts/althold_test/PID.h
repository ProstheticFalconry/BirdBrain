#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <pruss/prussdrv.h>
#include <pruss/pruss_intc_mapping.h>


#define CYCLES_PER_SEC 200000000
#define SPEED_OF_SOUND 34000 // in cm/s
#define k_P 0
#define k_I 0
#define k_D 0
#define I_max 100
#define time_step 5.0 // in ms
#define ms_per_sec 1000.0

#define MOT1 "/sys/class/pwm/pwmchip2/pwm0/duty_cycle"
#define MOT2 "/sys/class/pwm/pwmchip2/pwm1/duty_cycle"
#define MOT3 "/sys/class/pwm/pwmchip4/pwm0/duty_cycle"
#define MOT4 "/sys/class/pwm/pwmchip4/pwm1/duty_cycle"


class PID_class {
        public:
                PID_class(float);
                void set_altitude(float);
                void get_error(float);
                void calc_P();
                void calc_I();
                void calc_D();
                float update_throttle();
        private:
                float delta_throttle;
                float throttle;
                float error;
                float last_error;
                float d_error;
                float error_sum;
                float target_alt;
};

class PRU {
	public:
		PRU();
		float get_alt();
	private:
		unsigned int * pruData;
		float altitude;

};

class PWM4 {
	public:
		PWM4();
		void set_duty_cycle(float);
	private:
		std::ofstream fd1, fd2, fd3, fd4;
		int duty_cycle;
		char buff[20] = {0};
};                      
