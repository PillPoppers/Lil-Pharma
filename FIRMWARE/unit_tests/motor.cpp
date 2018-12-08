/*
 * motor.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: maxim
 */
#include "motor.h"
#include "hw_functions.h"

volatile uint32_t motor::motorPos = 0; // counts up to 2^32-1 before
volatile uint8_t motor::pwmInternal = 0;
uint32_t  motor::targetPos = 0;
int       motor::torque = 0;
int       motor::err = 0;
int       motor::posCtrlEn = 0;

//uint8_t   motor::inv_P_gain = 2;

// TODO: hard coded, should be fixed by adding a wrapper function in hw_functions.h for GPIO here.
void motor::forward(){
    GPIOPinWrite(GPIOA0_BASE, 0x20 | 0x40, 0x20); // MOTOR_IN_1 HIGH, MOTOR_IN_2 LOW
}

void motor::backward(){
    GPIOPinWrite(GPIOA0_BASE, 0x20 | 0x40, 0x40); // MOTOR_IN_1 LOW, MOTOR_IN_2 HIGH
}

void motor::stop(){
    GPIOPinWrite(GPIOA0_BASE, 0x20 | 0x40, 0x00); // MOTOR_IN_1 LOW, MOTOR_IN_2 LOW
}



void motor::update(){
    // update torque to reduce error
    err  = targetPos - motorPos;
//    err /= inv_P_gain;

    torque = err;
    pwmInternal = (pwmInternal < 0b11111) ? (pwmInternal + 1) : 0; //called by millis ISR.
    if(pwmInternal > 0 && pwmInternal <= torque)
        forward();
    else if(pwmInternal > 0 && pwmInternal <= -torque)
        backward();
    else
        stop();

}

