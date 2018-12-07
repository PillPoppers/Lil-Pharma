/*
 * pwmTimer.h
 *
 *  Created on: Nov 18, 2018
 *      Author: benja
 */

//NOT TIMERA0 BASE
//1-2 ms period

#define SERVO_RELOAD_VALUE      65536
#define BUZZER_RELOAD_VALUE     40000

void InitTimerA0_SERVO(void);
void OpenDoor(void);
void CloseDoor(void);
void InitTimer2B_BUZZER(void);


