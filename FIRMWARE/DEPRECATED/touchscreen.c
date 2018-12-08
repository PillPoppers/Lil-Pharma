// Driverlib includes
#include "utils.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "hw_adc.h"
#include "hw_ints.h"
#include "hw_gprcm.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart.h"
#include "pinmux.h"
#include "pin.h"
#include "adc.h"


#define NO_OF_SAMPLES 		8

unsigned long sampleX[8];
unsigned long sampleY[8];
unsigned long sampleA, sampleB;


unsigned long  adcPinX = PIN_60;
unsigned long yp = PIN_60;
unsigned long  adcPinY = PIN_58;
unsigned long  xm = PIN_58;

// Assign pins
unsigned long xp;
unsigned long ym;

// Assign proper factor to get the pixel (accurate to 1's digit) value back
unsigned long factor = 16450000;

unsigned int  adcChannelX = ADC_CH_3;
unsigned int  adcChannelY = ADC_CH_1;
unsigned int  indexA=0;
unsigned int  indexB=0;



int AnalogReadX()
{
    indexA = 0;

    MAP_PinTypeADC(adcPinX, PIN_MODE_255);
    MAP_ADCTimerConfig(ADC_BASE, 2 ^ 17);

    // Get data for X channel
    MAP_ADCTimerEnable(ADC_BASE);
    MAP_ADCEnable(ADC_BASE);
    MAP_ADCChannelEnable(ADC_BASE, adcChannelX);
    while(indexA < NO_OF_SAMPLES)
    {
        if(MAP_ADCFIFOLvlGet(ADC_BASE, adcChannelX))
        {
            sampleA = MAP_ADCFIFORead(ADC_BASE, adcChannelX);
            sampleX[indexA++] = (sampleA<<18)/factor;
        }
    }
    MAP_ADCChannelDisable(ADC_BASE, adcChannelX);
    MAP_ADCDisable(ADC_BASE);
    MAP_ADCTimerDisable(ADC_BASE);

    return sampleX[5]+sampleX[6]+sampleX[7];
}

int AnalogReadY()
{
    indexB = 0;
    MAP_PinTypeADC(adcPinY, PIN_MODE_255);
    MAP_ADCTimerConfig(ADC_BASE, 2 ^ 17);

    // Get data for X channel
    MAP_ADCTimerEnable(ADC_BASE);
    MAP_ADCEnable(ADC_BASE);
    MAP_ADCChannelEnable(ADC_BASE, adcChannelY);
    while(indexB < NO_OF_SAMPLES)
    {
        if(MAP_ADCFIFOLvlGet(ADC_BASE, adcChannelY))
        {
            sampleB = MAP_ADCFIFORead(ADC_BASE, adcChannelY);
            sampleY[indexB++] = (sampleB<<18)/factor;
        }
    }
    MAP_ADCChannelDisable(ADC_BASE, adcChannelY);
    MAP_ADCDisable(ADC_BASE);
    MAP_ADCTimerDisable(ADC_BASE);

    return sampleY[5]+sampleY[6]+sampleY[7];
}

int GetX()
{
    PinDirModeSet(yp, PIN_DIR_MODE_IN);
    PinDirModeSet(ym, PIN_DIR_MODE_IN);
    GPIOPinWrite(GPIOA2_BASE, yp, 0);
    //digitalWrite(_yp, LOW);
    GPIOPinWrite(GPIOA2_BASE, ym, 0);
    //digitalWrite(_ym, LOW);

    PinDirModeSet(xp, PIN_DIR_MODE_OUT);
    GPIOPinWrite(GPIOA2_BASE, xp, 1);
    // digitalWrite(_xp, HIGH);
    PinDirModeSet(xm, PIN_DIR_MODE_OUT);
    GPIOPinWrite(GPIOA2_BASE, xm, 0);
    //digitalWrite(_xm, LOW);

    // yp
    return AnalogReadX();
}

int GetY()
{
    PinDirModeSet(xp, PIN_DIR_MODE_IN);
    PinDirModeSet(xm, PIN_DIR_MODE_IN);
    GPIOPinWrite(GPIOA2_BASE, xp, 0);
    //digitalWrite(_xp, LOW);
    GPIOPinWrite(GPIOA2_BASE, xm, 0);
    //digitalWrite(_xm, LOW);

    PinDirModeSet(yp, PIN_DIR_MODE_OUT);
    GPIOPinWrite(GPIOA2_BASE, yp, 1);
    //digitalWrite(_yp, HIGH);
    PinDirModeSet(ym, PIN_DIR_MODE_OUT);
    GPIOPinWrite(GPIOA2_BASE, ym, 0);
    //digitalWrite(_ym, LOW);


    // xm
    return AnalogReadY();
}
