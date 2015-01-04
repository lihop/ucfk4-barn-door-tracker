#include "system.h"
#include "button.h"
#include "task.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "stepper.h"

#define MAX_SPEED 800 
#define PACER_RATE 130


int main (void)
{
    system_init ();
    stepper_init (4096, PD1_PIO, PD2_PIO, PD3_PIO, PD4_PIO);

    while (1)
    {
        stepper_step ();
    }

    return 0;
}
