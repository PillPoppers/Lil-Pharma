/*
 * pwmTimer.c
 *
 *  Created on: Nov 18, 2018
 *      Author: benja
 */

// Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "pwmTimer.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_timer.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
#include "timer.h"

void InitTimerA0_SERVO() {

    //configure the timer mode
    TimerConfigure(TIMERA0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM));

    //set prescale value: unit time = 0.3125 micro sec, max time = 20.48 ms
    TimerPrescaleSet(TIMERA0_BASE, TIMER_A, 25);

    //invert outputs if needed
    TimerControlLevel(TIMERA0_BASE, TIMER_A , 1);

    //set reload value (signal HIGH)
    TimerLoadSet(TIMERA0_BASE, TIMER_A, SERVO_RELOAD_VALUE);

    //set match value (signal LOW)
    TimerMatchSet(TIMERA0_BASE, TIMER_A, 65535); //fix last parameter here. max is 65535. Set to same as CloseDoor

    TimerPrescaleMatchSet(TIMERA0_BASE, TIMER_A, 1);

    //enable timer
    TimerEnable(TIMERA0_BASE, TIMER_A);
}

void OpenDoor() {
    TimerDisable(TIMERA0_BASE, TIMER_A);

    TimerMatchSet(TIMERA0_BASE, TIMER_A, 65535); //fix last parameter here. max is 65535

    TimerPrescaleMatchSet(TIMERA0_BASE, TIMER_A, 2);

    TimerEnable(TIMERA0_BASE, TIMER_A);
}

void CloseDoor() {
    TimerDisable(TIMERA0_BASE, TIMER_A);

    TimerMatchSet(TIMERA0_BASE, TIMER_A, 65535); //fix last parameter here. 65535 is max

    TimerPrescaleMatchSet(TIMERA0_BASE, TIMER_A, 1);

    TimerEnable(TIMERA0_BASE, TIMER_A);
}

void InitTimer2B_BUZZER() {
    //configure the timer mode
    TimerConfigure(TIMERA2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    //adjust this for right frequency
    //set prescale value: unit time = 0.3125 micro sec, max time = 20.48 ms
    TimerPrescaleSet(TIMERA2_BASE, TIMER_B, 0);

    //invert outputs if needed
    TimerControlLevel(TIMERA2_BASE, TIMER_B , 1);

    //adjust for right frequency
    //set reload value (signal HIGH)
    TimerLoadSet(TIMERA2_BASE, TIMER_B, BUZZER_RELOAD_VALUE);

    //adjust for right frequency
    //set match value (signal LOW)
    TimerMatchSet(TIMERA2_BASE, TIMER_B, (BUZZER_RELOAD_VALUE / 2));

    //adjust for right frequency
    TimerPrescaleMatchSet(TIMERA2_BASE, TIMER_B, 0);

    //enable timer
    TimerEnable(TIMERA2_BASE, TIMER_B);
}
