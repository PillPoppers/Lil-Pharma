#ifndef HW_FUNCTIONS_H_
#define HW_FUNCTIONS_H_

// common includes:
#include "stdint.h"
#include "stdio.h"

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
#include "uart_if.h"
#include "timer_if.h"
#include "i2c_if.h"
#include "i2c_if.h"

// useful misc includes
#include "defines.h"

#if defined(ccs)
extern void (*const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//functions!
void PinMuxConfig();
void Open_door();
void Close_door();
void buzz_enable();
void buzz_disable();
void InitTimerA0_SERVO();
void InitTimer2B_BUZZER();
void TimerBaseIntHandler(void);
void init_millis_timer();
void PHOTO_IN_handler(void);
void Encoder_I_Handler(void);
void Encoder_Q_Handler(void);

#endif /* HW_FUNCTIONS_H_ */
