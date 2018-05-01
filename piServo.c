#include <wiringPi.h>
#include <stdio.h>

int main (void)
{
    printf ("Raspberry Pi wiringPi test program\n");
    wiringPiSetupGpio();
    pinMode (3, PWM_OUTPUT) ;
    pwmSetMode (PWM_MODE_MS);
    pwmSetRange (2000);
    pwmSetClock (192);
    pwmWrite(3,150);
    
    delay(1000);
    pwmWrite(3,200);
    return 0;
}
