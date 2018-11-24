/*
 * pwmTimer.h
 *
 *  Created on: Nov 18, 2018
 *      Author: benja
 */

//NOT TIMERA0 BASE
//1-2 ms period

#define RELOAD_VALUE    65536

void InitTimer2B_PWM(void);
void OpenDoor(void);
void CloseDoor(void);


