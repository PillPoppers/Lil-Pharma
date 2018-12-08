#include <msp430.h>
#include <util.h>
#include "LED.h"
#include "spi.h"
#include "LCD.h"
#include "bitmaps.h"


void ConfigureClockModule()
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    //ConfigureClockModule();

    InitializeLEDPortPins();
    InitializeSPI();
    InitializeLCD();

    _delay_cycles(500000);

    drawBitmap(0, 0, 50, 50, 2, Todd);

    while (1) {
        _delay_cycles(500000);

        TOGGLE_LED1;

    }
}



