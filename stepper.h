/*
    Drives a unipolar stepper motor using 4 wires.
    Uses 8 step sequencing for maximum resolution.
    Specifically designed for stepper motor model 28BYJ-48,
    but might work with other models.
*/


#ifndef STEPPER_H
#define STEPPER_H

#include "pio.h"

/* Define the possible directions of motion */
typedef enum stepper_direction {
    CW,  // clockwise
    CCW, // counterclockwise
} stepper_direction_t;

/* Move the stepper motor one step */
void stepper_step (void);

/* Set the direction of motion */
void stepper_direction_set (stepper_direction_t direction);

/* Initialise the stepper motor */
void stepper_init (uint16_t number_of_steps, pio_t motor_pin_1,
    pio_t motor_pin_2, pio_t motor_pin_3, pio_t motor_pin_4);

#endif
