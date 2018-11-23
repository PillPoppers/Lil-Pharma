/*
 * pwmTimer.c
 *
 *  Created on: Nov 18, 2018
 *      Author: benja
 */

//NOT TIMERA0 BASE
//1-2 ms period

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

void InitTimer2B_PWM() {

    //configure the timer mode
    TimerConfigure(TIMERA2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    //set prescale value: unit time = 0.3125 micro sec, max time = 20.48 ms
    TimerPrescaleSet(TIMERA2_BASE, TIMER_B, 25);

    //invert outputs if needed
    TimerControlLevel(TIMERA2_BASE, TIMER_B , 1);

    //set reload value (signal HIGH)
    TimerLoadSet(TIMERA2_BASE, TIMER_B, RELOAD_VALUE);

    //set match value (signal LOW)
    TimerMatchSet(TIMERA2_BASE, TIMER_B, 65535); //fix last parameter here. max is 65535. Set to same as CloseDoor

    TimerPrescaleMatchSet(TIMERA2_BASE, TIMER_B, 1);

    //enable timer
    TimerEnable(TIMERA2_BASE, TIMER_B);
}

void OpenDoor() {
    TimerDisable(TIMERA2_BASE, TIMER_B);

    TimerMatchSet(TIMERA2_BASE, TIMER_B, 65535); //fix last parameter here. max is 65535

    TimerPrescaleMatchSet(TIMERA2_BASE, TIMER_B, 2);

    TimerEnable(TIMERA2_BASE, TIMER_B);
}

void CloseDoor() {
    TimerDisable(TIMERA2_BASE, TIMER_B);

    TimerMatchSet(TIMERA2_BASE, TIMER_B, 65535); //fix last parameter here. 65535 is max

    TimerPrescaleMatchSet(TIMERA2_BASE, TIMER_B, 1);

    TimerEnable(TIMERA2_BASE, TIMER_B);
}


