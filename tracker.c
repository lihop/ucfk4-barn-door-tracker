#include "system.h"
#include "navswitch.h"
#include "task.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "stepper.h"

/* Define polling rates in Hz */
#define NAVSWITCH_TASK_RATE 100
#define DISPLAY_TASK_RATE 250
#define TIMER_TASK_RATE 1
#define STEPPER_TASK_RATE 1000

#define DEFAULT_STATUS IDLE
#define DEFAULT_TARGET_TIME 0 
#define MAX_TARGET_TIME 99
#define DEFAULT_DIRECTION CW


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
    tracker_state_t *tracker = data;

    int time;
    char str[3];

    if (tracker->status == IDLE)
        time = tracker->target_time;
    else
        time = tracker->target_time - tracker->elapsed_time;

    str[0] = ((time / 10) % 10) + '0';
    str[1] = (time % 10) + '0';
    str[2] = '\0';

    tinygl_text (str);
    tinygl_update ();
}

static void navswitch_task_init (void)
{
    navswitch_init ();
}

static void navswitch_task (void *data)
{
    tracker_state_t *tracker = data;

    int target_time = tracker->target_time;

    navswitch_update ();

    if (tracker->status == IDLE)
    {
        if (navswitch_push_event_p (NAVSWITCH_NORTH))
            tracker->target_time = ((target_time + 1) % (MAX_TARGET_TIME + 1));
        
        else if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        {
            tracker->target_time --;

            // Wrap around to MAX_TARGET_TIME if new target time is negative
            if (tracker->target_time < 0)
                tracker->target_time += (MAX_TARGET_TIME +1);
        }
        
        else if (navswitch_push_event_p (NAVSWITCH_PUSH))
            tracker->status = TRACKING;
    }
    else
    {
        if (navswitch_push_event_p (NAVSWITCH_PUSH))
            tracker->status = !(tracker->status);
    }
}

static void timer_task_init (void)
{
}

static void timer_task (void *data)
{
    tracker_state_t *tracker = data;
    
    static int seconds = 0;

    if (tracker->status == TRACKING)
    {
        if (tracker->elapsed_time >= tracker->target_time)
        {
            tracker->status = !(tracker->status);
            tracker->elapsed_time = 0;
            seconds = 0;
        }
        else
        {
            if (seconds == 60)
            {
                tracker->elapsed_time ++; 
                seconds = 0;
            }
            else
            {
                seconds ++;
            }
        }
    }
}

static void stepper_task_init (void)
{
    stepper_init (4096, PD1_PIO, PD2_PIO, PD3_PIO, PD4_PIO);
}

static void stepper_task (void *data)
{
    tracker_state_t *tracker = data;

    if (tracker->status == TRACKING)
    {
        stepper_step ();
    }
}

int main (void)
{
    tracker_state_t tracker =
    {
        .status = DEFAULT_STATUS,
        .direction = DEFAULT_DIRECTION,
        .target_time = DEFAULT_TARGET_TIME,
        .elapsed_time = 0,
    };

    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE,
            .data = &tracker},
        {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE,
            .data = &tracker},
        {.func = timer_task, .period = TASK_RATE / TIMER_TASK_RATE,
            .data = &tracker}, 
        {.func = stepper_task, .period = TASK_RATE / STEPPER_TASK_RATE,
            .data = &tracker},
    };

    /* Make tracker.speed a pointer to the period of the stepper task so that the
     * stepper motors speed can be adjusted at runtime. */
    tracker.speed = &((tasks+3)->period);

    system_init ();

    display_task_init ();
    navswitch_task_init ();
    timer_task_init ();
    stepper_task_init ();
    
    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
