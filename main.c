#include "system.h"
#include "stepper.h"
#include "pacer.h"

#define MAX_SPEED 800 
#define PACER_RATE 130


int main (void)
{
    system_init ();
    stepper_init (4096, PD1_PIO, PD2_PIO, PD3_PIO, PD4_PIO);

    pacer_init (PACER_RATE);

    while (1)
    {
        pacer_wait ();
        stepper_step ();
    }

    return 0;
}
