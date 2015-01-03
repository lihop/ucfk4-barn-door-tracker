#include "stepper.h"

typedef struct stepper_state_struct
{
    uint8_t step_number; // which step in a full revolution the motor is on
    stepper_direction_t direction; // direction of rotation
    uint16_t number_of_steps; // total number of steps this motor can take
    pio_t motor_pin_1;
    pio_t motor_pin_2;
    pio_t motor_pin_3;
    pio_t motor_pin_4;
} stepper_state_t;

static stepper_state_t stepper;

/* Move the stepper motor forward or backwards */
void step_motor (int step)
{
    switch (step)
    {
        case 0: // 1000
            pio_output_high (stepper.motor_pin_1);
            pio_output_low  (stepper.motor_pin_2);
            pio_output_low  (stepper.motor_pin_3);
            pio_output_low  (stepper.motor_pin_4);
            break;
        case 1: // 1100 ;
            pio_output_high (stepper.motor_pin_1);
            pio_output_high (stepper.motor_pin_2);
            pio_output_low  (stepper.motor_pin_3);
            pio_output_low  (stepper.motor_pin_4);
            break;
        case 2: // 0100;
            pio_output_low  (stepper.motor_pin_1);
            pio_output_high (stepper.motor_pin_2);
            pio_output_low  (stepper.motor_pin_3);
            pio_output_low  (stepper.motor_pin_4);
            break;
        case 3: // 0110 ;
            pio_output_low  (stepper.motor_pin_1);
            pio_output_high (stepper.motor_pin_2);
            pio_output_high (stepper.motor_pin_3);
            pio_output_low  (stepper.motor_pin_4);
            break;
        case 4: // 0010 ;
            pio_output_low (stepper.motor_pin_1);
            pio_output_low  (stepper.motor_pin_2);
            pio_output_high (stepper.motor_pin_3);
            pio_output_low  (stepper.motor_pin_4);
            break;
        case 5: // 0011 ;
            pio_output_low  (stepper.motor_pin_1);
            pio_output_low  (stepper.motor_pin_2);
            pio_output_high (stepper.motor_pin_3);
            pio_output_high (stepper.motor_pin_4);
            break;
        case 6: // 0001 ;
            pio_output_low  (stepper.motor_pin_1);
            pio_output_low  (stepper.motor_pin_2);
            pio_output_low  (stepper.motor_pin_3);
            pio_output_high (stepper.motor_pin_4);
            break;
        case 7: // 1001;
            pio_output_high (stepper.motor_pin_1);
            pio_output_low  (stepper.motor_pin_2);
            pio_output_low  (stepper.motor_pin_3);
            pio_output_high (stepper.motor_pin_4);
            break;
    }
}

/* Set the direction of circular motion */
void stepper_direction_set (stepper_direction_t direction)
{
    stepper.direction = direction;
}

/* Move the stepper motor one step */
void stepper_step (void)
{
    /* Increment or decrement the step number,
       depending on direction */
    if (stepper.direction == 0)
    {
        stepper.step_number++;
        if (stepper.step_number == stepper.number_of_steps)
            stepper.step_number = 0;
    }
    
    else
    {
        if (stepper.step_number == 0)
            stepper.step_number = stepper.number_of_steps;
        stepper.step_number--;
    }

    // step the motor to step number 0, 1, 2, 3, 4, 5, 6, or 7:
    step_motor (stepper.step_number % 8);
}

/* Initialise things */
void stepper_init (uint8_t number_of_steps, pio_t motor_pin_1,
    pio_t motor_pin_2, pio_t motor_pin_3, pio_t motor_pin_4)
{
    stepper.number_of_steps = number_of_steps;
    stepper.motor_pin_1 = motor_pin_1;
    stepper.motor_pin_2 = motor_pin_2;
    stepper.motor_pin_3 = motor_pin_3;
    stepper.motor_pin_4 = motor_pin_4;
    pio_config_set (stepper.motor_pin_1, PIO_OUTPUT_LOW);
    pio_config_set (stepper.motor_pin_2, PIO_OUTPUT_LOW);
    pio_config_set (stepper.motor_pin_3, PIO_OUTPUT_LOW);
    pio_config_set (stepper.motor_pin_4, PIO_OUTPUT_LOW);

}
