/*
 * arduino.h
 *
 *  Created on: Oct 26, 2018
 *      Author: maxim
 */

// Standard includes
#include <string.h>
#include <stdint.h>
#include <stdio.h>

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
#include "timer.h"

#ifndef ARDUINO_H_
#define ARDUINO_H_

// define some basic functions:
#define min(a,b) ((a)>(b)?(b):(a))
#define max(a,b) ((a)>(b)?(a):(b))
#define delay(i) MAP_UtilsDelay(14000*i)

#define F(string_literal) (reinterpret_cast<__FlashStringHelper *>(PSTR(string_literal)))






// use arduino macro if you prefer:
#define OUTPUT GPIO_DIR_MODE_OUT
#define INPUT  GPIO_DIR_MODE_IN
#define HIGH 1
#define LOW  0

// note: The CC3200 is not an AVR ATmegaxxx MCU. So the following functions are similar to arduino, but not exactly the same. (maybe different arguments.)


// digitalWrite, similar to arduino, will take a pin number and will write it HIGH or LOW.
// it just abstracts the GPIOPinWrite function a bit in a arduino library-friendly way.

// the GPIO pin is specified based on the labeling given on the datasheet. (GPIO 0 -- 18, 22 --
// (it seems that GPIOPinWrite is really just a thin abstraction of raw port manipulation.
//void digitalWrite(uint16_t pin, bool state){
//
//    GPIOPinWrite()
//}


//*****************************************************************************
//
//! The interrupt is used to implement millis() and micros() and can be modified
//! to allow software PWM to be implemented on any pin. for the first timer interrupt.
//!
//! \param  None
//! \return none
//
//*****************************************************************************


#endif /* ARDUINO_H_ */
