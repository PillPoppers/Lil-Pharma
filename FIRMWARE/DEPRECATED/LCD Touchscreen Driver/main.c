// Driverlib includes
#include "pinmux.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"


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
#include "touchscreen.h"

unsigned int x, y;

static void BoardInit(void)
{
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}


void main()
{
    BoardInit();

    PinMuxConfig();
    InitTerm();

    while(1)
    {

        //x = GetX();
        //y = GetY();

        //y = AnalogReadY();
        //x = AnalogReadX();
        //UtilsDelay(5000);

        //UtilsDelay(5000);

//                GPIOPinWrite(GPIOA0_BASE, PIN_58, PIN_58);
//                GPIOPinWrite(GPIOA0_BASE, PIN_60, PIN_60);
//                GPIOPinWrite(GPIOA1_BASE, PIN_63, PIN_63);
//                GPIOPinWrite(GPIOA1_BASE, PIN_64, PIN_64);
//
//                UtilsDelay(5000000);
//
//
//                GPIOPinWrite(GPIOA0_BASE, PIN_58, 0);
//                GPIOPinWrite(GPIOA0_BASE, PIN_60, 0);
//                GPIOPinWrite(GPIOA1_BASE, PIN_63, 0);
//                GPIOPinWrite(GPIOA1_BASE, PIN_64, 0);
//
//                UtilsDelay(5000000);



    }
}
