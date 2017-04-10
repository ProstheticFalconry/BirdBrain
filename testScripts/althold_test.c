#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>



/******************
* Initialize PWMs *
*******************/

char *_generate_export_path(uint8_t chip)
{
    char *path;
    int r = asprintf(&path, "/sys/class/pwm/pwmchip%u/export", chip);
    if (r == -1) {
        printf("couldn't allocate export path\n");
    }
    return path;
}

char *_generate_polarity_path(uint8_t chip, uint8_t channel)
{
    char *path;
    int r = asprintf(&path, "/sys/class/pwm/pwmchip%u/pwm%u/polarity",
                     chip, channel);
    if (r == -1) {
        printf("couldn't allocate polarity path\n");
    }
    return path;
}

char * _generate_enable_path(uint8_t chip, uint8_t channel)
{
    char *path;
    int r = asprintf(&path, "/sys/class/pwm/pwmchip%u/pwm%u/enable",
                     chip, channel);
    if (r == -1) {
        printf("couldn't allocate enable path\n");
    }
    return path;
}

char * _generate_duty_path(uint8_t chip, uint8_t channel)
{
    char *path;
    int r = asprintf(&path, "/sys/class/pwm/pwmchip%u/pwm%u/duty_cycle",
                     chip, channel);
    if (r == -1) {
        printf("couldn't allocate duty path\n");
    }
    return path;
}

char * _generate_period_path(uint8_t chip, uint8_t channel)
{
    char *path;
    int r = asprintf(&path, "/sys/class/pwm/pwmchip%u/pwm%u/period",
                     chip, channel);
    if (r == -1) {
        printf("couldn't allocate period path\n");
    }
    return path;
}

int _pwm_init(int period, int duty_cycle)
{
    /* Start up all 4 pwm signals with freq and duty cycle */
    /* input period and duty_cycle as usec, so convert to nsec */
    period = period * 1000; // period in nsec
    duty_cycle = duty_cycle * 1000;  // duty_cycle in nsec

    // export everything and initialize PWMs

}

/****************
* Initialize PRUs
****************/

int main()
{
    _pwm_init(2000, 1000);
    _pru_init();


    return 0;
}

