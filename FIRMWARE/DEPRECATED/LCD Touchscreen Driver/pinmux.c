
#include "pinmux.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"

//*****************************************************************************
void
PinMuxConfig(void)
{
    //
    // Enable Peripheral Clocks
    //
    PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);

    PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);

    //
    // Configure PIN_58 for GPIO Output
    //
    PinTypeGPIO(PIN_58, PIN_MODE_0, false);

    //
    // Configure PIN_60 for GPIO Output
    //
    PinTypeGPIO(PIN_60, PIN_MODE_0, false);

    //
    // Configure PIN_63 for GPIO Output
    //
    PinTypeGPIO(PIN_63, PIN_MODE_0, false);

    //
    // Configure PIN_64 for GPIO Output
    //
    PinTypeGPIO(PIN_64, PIN_MODE_0, false);

}
