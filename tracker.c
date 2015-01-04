#include "system.h"
#include "button.h"
#include "task.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "stepper.h"

/* Define polling rates in Hz */
#define BUTTON_TASK_RATE 100
#define DISPLAY_TASK_RATE 250
#define TIMER_TASK_RATE 100
#define STEPPER_TASK_RATE 100

#define MAX_SPEED 800 
#define PACER_RATE 130

typedef enum {
    IDLE,       // The tracker is not running and settings can be adjusted
    TRACKING,   // The tracker is running
} tracker_status_t;

typedef struct tracker_state {
    tracker_status_t status;
    stepper_direction_t direction;
    uint16_t *speed;
    int target_time;  // The number of minutes the tracker should run for
    int elapsed_time; // The number of minutes the tracker has been running
} tracker_state_t;

static void display_task_init (void)
{
    tinygl_init (DISPLAY_TASK_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}

static void display_task (__unused__ void *data)
{
    tinygl_update ();
}

static void button_task_init (void)
{
    button_init ();
}

static void button_task (__unused__ void *data)
{
    button_update ();
}

static void timer_task_init (void)
{
}

static void timer_task (__unused__ void *data)
{
}

static void stepper_task_init (void)
{
    stepper_init (4096, PD1_PIO, PD2_PIO, PD3_PIO, PD4_PIO);
}

static void stepper_task (__unused__ void *data)
{
}

int main (void)
{
    tracker_state_t tracker;

    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE,
            .data = &tracker},
        {.func = button_task, .period = TASK_RATE / BUTTON_TASK_RATE,
            .data = &tracker},
        {.func = timer_task, .period = TASK_RATE / BUTTON_TASK_RATE,
            .data = &tracker}, 
        {.func = stepper_task, .period = TASK_RATE / STEPPER_TASK_RATE,
            .data = &tracker},
    };

    /* Make tracker.speed a pointer to the period of the stepper task so that the
     * stepper motors speed can be adjusted at runtime. */
    tracker.speed = &((tasks+3)->period);

    system_init ();

    display_task_init ();
    button_task_init ();
    timer_task_init ();
    stepper_task_init ();
    
    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
