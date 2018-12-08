#include "pinmux.h"
#include "hw_types.h"
#include "pin.h"
#include "prcm.h"
#include "gpio.h"
#include "hw_memmap.h"



//*****************************************************************************
void SPIPinMuxConfig(void)
{
    // DCn
    // Enable Peripheral Clocks
    PRCMPeripheralClkEnable(PRCM_GPIOA2, PRCM_RUN_MODE_CLK);
    PinTypeGPIO(PIN_15, PIN_MODE_0, false);
    GPIODirModeSet(GPIOA2_BASE, 0x40, GPIO_DIR_MODE_OUT);
    //
    // Enable Peripheral Clocks
    //
    PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);

    //
    // Configure PIN_05 for SPI0 GSPI_CLK
    //
    PinTypeSPI(PIN_05, PIN_MODE_7);

    //
    // Configure PIN_07 for SPI0 GSPI_MOSI
    //
    PinTypeSPI(PIN_07, PIN_MODE_7);

    //
    // Configure PIN_08 for SPI0 GSPI_CS
    //
    PinTypeSPI(PIN_08, PIN_MODE_7);
}

void SetDC()
{
    GPIOPinWrite(GPIOA2_BASE,GPIO_PIN_6,GPIO_PIN_6);
}

void ResetDC()
{
    GPIOPinWrite(GPIOA2_BASE,GPIO_PIN_6,0);
}


