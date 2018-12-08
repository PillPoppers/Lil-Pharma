/*
 * touch.c
 *
 *  Created on: Dec 7, 2018
 *      Author: maxim
 */

#include "touch.h"
#include "hw_functions.h"

int readX() {
    // set port direction registers and pin types to read the Y direction

    // YD -> FLOAT (HIGH-Z) (implemented by setting the output as OPEN DRAIN then HIGH.)
    PinTypeGPIO(PIN_YD, PIN_MODE_0, false);
    GPIODirModeSet(PORT_YD, MASK_YD, GPIO_DIR_MODE_IN);
    GPIOPinWrite(PORT_YD, MASK_YD, 0xFF);

    // YU -> ADC_IN
    PinTypeADC(PIN_YU, PIN_MODE_255);
    GPIODirModeSet(PORT_YU, MASK_YU, GPIO_DIR_MODE_IN);

    // XR -> LOW
    PinTypeGPIO(PIN_XR, PIN_MODE_0, false);
    GPIODirModeSet(PORT_XR, MASK_XR, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(PORT_XR, MASK_XR, 0x00);

    // XL -> HIGH
    PinTypeGPIO(PIN_XL, PIN_MODE_0, false);
    GPIODirModeSet(PORT_XL, MASK_XL, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(PORT_XL, MASK_XL, 0xFF);

    // Get data for X channel
    ADCTimerConfig(ADC_BASE, 2^17);
    ADCTimerEnable(ADC_BASE);
    ADCEnable(ADC_BASE);
    ADCChannelEnable(ADC_BASE, ADC_YU);
    delay(1); // wait for ADC hardware to enable

    uint32_t i, s = 0;
    for(i = 0; i < NO_OF_SAMPLES; i++) {
        if(ADCFIFOLvlGet(ADC_BASE, ADC_YU)) {
            uint32_t  sample = ADCFIFORead(ADC_BASE, ADC_YU);    // mask off all higher bits
            sample &= 0x3FFF;
            s += sample;
        }
        else{
            break;
        }
    }
    s = s / i;

    ADCChannelDisable(ADC_BASE, ADC_YU);
    ADCDisable(ADC_BASE);
    ADCTimerDisable(ADC_BASE);

    return s;
}

int readY() {

    // set port direction registers and pin types to read the Y direction

    // YD -> LOW
    PinTypeGPIO(PIN_YD, PIN_MODE_0, false);
    GPIODirModeSet(PORT_YD, MASK_YD, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(PORT_YD, MASK_YD, 0x00);

    // YU -> HIGH
    PinTypeGPIO(PIN_YU, PIN_MODE_0, false);
    GPIODirModeSet(PORT_YU, MASK_YU, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(PORT_YU, MASK_YU, 0xFF);

    // XR -> FLOAT (HIGH-Z)
    PinTypeGPIO(PIN_XR, PIN_MODE_0, false);
    GPIODirModeSet(PORT_XR, MASK_XR, GPIO_DIR_MODE_IN);

//     XL -> ADC_IN
    PinTypeADC(PIN_XL, PIN_MODE_255);
    GPIODirModeSet(PORT_XL, MASK_XL, GPIO_DIR_MODE_IN);

    // Get data for Y channel
    ADCTimerConfig(ADC_BASE, 2^17);
    ADCTimerEnable(ADC_BASE);
    ADCEnable(ADC_BASE);
    ADCChannelEnable(ADC_BASE, ADC_XL);
    delay(1);

    uint32_t i, s = 0;
    for(i = 0; i < NO_OF_SAMPLES; i++) {
        if(ADCFIFOLvlGet(ADC_BASE, ADC_XL)) {
            s += ADCFIFORead(ADC_BASE, ADC_XL) & 0x3FFF; // mask off all higher bits
        }
        else{
            break;
        }
    }
    s = s / i;

    ADCChannelDisable(ADC_BASE, ADC_XL);
    ADCDisable(ADC_BASE);
    ADCTimerDisable(ADC_BASE);

    return s;
}

