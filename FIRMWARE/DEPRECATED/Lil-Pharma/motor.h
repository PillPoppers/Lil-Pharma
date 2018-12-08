/*
 * motor.h
 *
 *  Created on: Nov 23, 2018
 *      Author: maxim
 */

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "spi.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "uart.h"
#include "interrupt.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"
#include "timer.h"
#include "adc.h"

// Common interface includes
#include "arduino.h"
#include "uart_if.h"
#include "timer_if.h"

#ifndef MOTOR_H_
#define MOTOR_H_


class motor {
public:
    static volatile uint32_t motor::motorPos;
    static volatile uint8_t pwmInternal;
    static int torque; // PWM of motor positive will make motor move forward negative makes motor go backward. Range is +/- 32.
    static uint32_t targetPos;
    static const uint8_t inv_P_gain = 2; // inverse P gain is better since I am working with discreet numbers and I will probably need to reduce the gain from unity given the resolution of the encoder.
    static int err;

    static void
        forward(),
        backward(),
        stop(),
        update();
};

#endif /* RELEASE_MOTOR_H_ */
