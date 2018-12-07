// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_apps_rcm.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "rom.h"
#include "rom_map.h"
#include "timer.h"
#include "utils.h"
#include "prcm.h"

#include "pinmux.h"
#include "pwmTimer.h"
#include "i2c_if.h"
#include "myi2c.h"
#include "rtc.h"
/**
 * main.c
 */

#define APPLICATION_VERSION     "1.1.1"
#define DBG_PRINT               Report

void (* const g_pfnVectors[])(void);

static void BoardInit(void){
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
  //
  // Set vector table base
  //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}



extern unsigned char RTCRead[];
extern unsigned char RTCWrite[];
extern unsigned char ConvertedTime[7];
extern unsigned char CurrentTime[7];


int main(void)
{
    int check = 0;
    BoardInit();

    PinMuxConfig();

    //InitTimer2B_BUZZER();

    //check = I2C_IF_Open(I2C_MASTER_MODE_STD);
    MyI2CInit();
    RTCInit();
    //I2C_IF_Open(0);

    CurrentTime[0] = 18;
    CurrentTime[1] = 11;
    CurrentTime[2] = 30;
    CurrentTime[3] = 5;
    CurrentTime[4] = 6;
    CurrentTime[5] = 15;
    CurrentTime[6] = 10;

    RTCSetTime();

    while(1) {
        RTCWrite[1] = 0;
        RTCWrite[2] = 0;
        RTCTimeGet();
    }
}
