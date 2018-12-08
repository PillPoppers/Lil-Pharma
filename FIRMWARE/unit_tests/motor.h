#ifndef MOTOR_H_
#define MOTOR_H_
/*
 * motor.h
 *
 *  Created on: Nov 23, 2018
 *      Author: maxim
 */
#include "stdint.h"

class motor {
public:
    static volatile uint32_t motorPos;
    static volatile uint8_t pwmInternal;
    static int torque; // PWM of motor positive will make motor move forward negative makes motor go backward. Range is +/- 32.
    static uint32_t targetPos;
    static int posCtrlEn; // enables/disables positional control of the motor.
    static const uint8_t inv_P_gain = 2; // inverse P gain is better since I am working with discreet numbers and I will probably need to reduce the gain from unity given the resolution of the encoder.
    static int err;

    static void
        forward(),
        backward(),
        stop(),
        update();
};

#endif /* RELEASE_MOTOR_H_ */
