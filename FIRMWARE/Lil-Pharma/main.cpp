//*****************************************************************************
//
// Application Name     - SPI Demo
// Application Overview - The demo application focuses on showing the required 
//                        initialization sequence to enable the CC3200 SPI 
//                        module in full duplex 4-wire master and slave mode(s).
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_SPI_Demo
// or
// docs\examples\CC32xx_SPI_Demo.pdf
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SPI_Demo
//! @{
//
//*****************************************************************************
// Standard includes
#include <string.h>
#include <stdint.h>

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
#include "arduino.h"
#include "uart_if.h"
#include "timer_if.h"

// modified arduino / adafruit library:
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "motor.h"

//*****************************************************************************
// GLOBAL VARIABLES -- Start
//*****************************************************************************
//static uint8_t g_ucTxBuff[64];
//static uint8_t g_ucRxBuff[64];
//static uint8_t ucTxBuffNdx;
//static uint8_t ucRxBuffNdx;

#define SPI_IF_BIT_RATE  1000000
#define TR_BUFF_SIZE     100
#define UART_BAUD_RATE   115200
#define PWM_NUM          3
volatile uint32_t millis   = 0;
volatile uint8_t I, Q;
static volatile unsigned long g_ulBase;


#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

void PHOTO_IN_handler(void);
void Encoder_I_Handler(void);
void Encoder_Q_Handler(void);
void TimerBaseIntHandler(void);

static void BoardInit(void) {

// In case of TI-RTOS vector table is initialize by OS itself
#ifndef USE_TIRTOS
  // Set vector table base
#if defined(ccs)
    MAP_IntVTableBaseSet((uint32_t)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((uint32_t)&__vector_table);
#endif
#endif
    // Enable Processor
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    PRCMCC3200MCUInit();
}


//TODO: JTAG pins are disabled so debugger works. ADC pins are disabled to prevent damage.
void PinMuxConfig(void) {

    // Set unused pins to PIN_MODE_0 with the exception of JTAG pins 16,17,19,20
    // Enable Peripheral Clocks
    PRCMPeripheralClkEnable(PRCM_ADC, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA2, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA3, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_TIMERA1, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK);

    PinTypeGPIO(PIN_03, PIN_MODE_0, false); // Configure PIN_03 for GPIO Output (GPIO12, pin_09 on MOD, LCD_RS)
    GPIODirModeSet(GPIOA1_BASE, 0x10, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_04, PIN_MODE_0, false); // Configure PIN_04 for GPIO Output (GPIO12, pin_10 on MOD, LCD_YD)
    GPIODirModeSet(GPIOA1_BASE, 0x20, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_15, PIN_MODE_0, false); // Configure PIN_15 for GPIO Output (GPIO22, pin_11 on MOD, LCD_XD)
    GPIODirModeSet(GPIOA2_BASE, 0x40, GPIO_DIR_MODE_OUT);
//    PinTypeGPIO(PIN_16, PIN_MODE_0, false); // Configure PIN_16 for GPIO Output (JTAG_TDI, pin_12 on MOD, solenoid driver)
//    GPIODirModeSet(GPIOA2_BASE, 0x80, GPIO_DIR_MODE_OUT);
//    PinTypeGPIO(PIN_17, PIN_MODE_0, false); // Configure PIN_17 for GPIO Output (JTAG_TDO, pin_18 on MOD, for servo)
//    GPIODirModeSet(GPIOA3_BASE, 0x1, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_18, PIN_MODE_0, false); // Configure PIN_18 for GPIO Output (GPIO28, pin_19 on MOD, nSLEEP)
    GPIODirModeSet(GPIOA3_BASE, 0x10, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_50, PIN_MODE_0, false); // Configure PIN_50 for GPIO Output (GPIO00, pin_44 on MOD, LCD_RESET)
    GPIODirModeSet(GPIOA0_BASE, 0x1, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_60, PIN_MODE_0, false); // Configure PIN_60 for GPIO Output (GPIO05, pin_50 on MOD, MOTOR_IN_1)
    GPIODirModeSet(GPIOA0_BASE, 0x20, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_61, PIN_MODE_0, false); // Configure PIN_61 for GPIO Output (GPIO06, pin_51 on MOD, MOTOR_IN_2)
    GPIODirModeSet(GPIOA0_BASE, 0x40, GPIO_DIR_MODE_OUT);

    PinTypeGPIO(PIN_53, PIN_MODE_0, false); // Configure PIN_53 for GPIO INT Input (GPIO30, pin_42 on MOD, PHOTO_IN)
    GPIOIntTypeSet(GPIOA3_BASE, 0x1, GPIO_BOTH_EDGES);
    GPIODirModeSet(GPIOA3_BASE, 0x40, GPIO_DIR_MODE_IN);
    GPIOIntRegister(GPIOA3_BASE, PHOTO_IN_handler);
    GPIOIntEnable(GPIOA3_BASE, 0x1);

    PinTypeGPIO(PIN_62, PIN_MODE_0, false); // Configure PIN_62 for GPIO INT Input (GPIO07, pin_53 on MOD, ENCODER_I)
    GPIODirModeSet(GPIOA0_BASE, 0x80, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(GPIOA0_BASE, 0x80, GPIO_BOTH_EDGES);
    GPIOIntRegister(GPIOA0_BASE, Encoder_I_Handler);
    GPIOIntEnable(GPIOA0_BASE, 0x1);

    PinTypeGPIO(PIN_63, PIN_MODE_0, false); // Configure PIN_63 for GPIO INT Input (GPIO08, pin_54 on MOD, ENCODER_Q)
    GPIODirModeSet(GPIOA1_BASE, 0x1, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(GPIOA1_BASE, 0x1, GPIO_BOTH_EDGES);
    GPIOIntRegister(GPIOA1_BASE, Encoder_Q_Handler);
    GPIOIntEnable(GPIOA1_BASE, 0x1);



    PinTypeI2C(PIN_01, PIN_MODE_1); // Configure PIN_01 for I2C0 I2C_SCL
    PinTypeI2C(PIN_02, PIN_MODE_1); // Configure PIN_02 for I2C0 I2C_SDA

    PinTypeGPIO(PIN_08, PIN_MODE_0, false);
    GPIODirModeSet(GPIOA2_BASE, 0x2, GPIO_DIR_MODE_OUT); // use software-defined CS pin
    //PinTypeSPI(PIN_08, PIN_MODE_7); // Configure PIN_08 (pin 8 also for MOD) for SPI0 GSPI_CS
    PinTypeSPI(PIN_05, PIN_MODE_7);   // Configure PIN_05 (pin 5 also for MOD) for SPI0 GSPI_CLK
    PinTypeSPI(PIN_06, PIN_MODE_7);   // Configure PIN_06 (pin 6 also for MOD) for SPI0 GSPI_MISO
    PinTypeSPI(PIN_07, PIN_MODE_7);   // Configure PIN_07 (pin 7 also for MOD) for SPI0 GSPI_MOSI

    PinTypeUART(PIN_55, PIN_MODE_3);  // Configure PIN_55 for UART0 UART0_TX
    PinTypeUART(PIN_57, PIN_MODE_3);  // Configure PIN_57 for UART0 UART0_RX

    PinTypeTimer(PIN_64, PIN_MODE_3); // Configure PIN_64 for TimerPWM5 GT_PWM05 (buzzer)
    PinTypeTimer(PIN_21, PIN_MODE_9); // Configure PIN_21 for TimerPWM2 GT_PWM02 (LCD backlight)

    // Initialize GPT A0 (in 32 bit mode) as periodic down counter.
    PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_TIMERA0);
    TimerConfigure(TIMERA0_BASE, TIMER_CFG_PERIODIC);
    TimerPrescaleSet(TIMERA0_BASE, TIMER_A, 0);

    PinTypeADC(PIN_58, PIN_MODE_255); // Configure PIN_58 for ADC0 ADC_CH1 (LCD touchscreen) 1.5V MAXIMUM
    PinTypeADC(PIN_59, PIN_MODE_255); // Configure PIN_59 for ADC0 ADC_CH2 (LCD touchscreen) 1.5V MAXIMUM
    ADCEnable(ADC_BASE);              // enable ADC
}

 void main() {

    BoardInit(); // Initialize Board configurations
    PinMuxConfig(); // Muxing UART and SPI lines.
    PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK); // Enable SPI module clock
    InitTerm(); // Initialising the Terminal.
    ClearTerm(); // Clearing the Terminal.

    PRCMPeripheralReset(PRCM_GSPI); // Reset the peripheral

    g_ulBase = TIMERA0_BASE; // Base address for first timer
    Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0); // Configuring the timers
    Timer_IF_IntSetup(g_ulBase, TIMER_A, TimerBaseIntHandler); // Setup the interrupts for the timer timeouts.
    Timer_IF_Start(g_ulBase, TIMER_A, 1); // Turn on the timers feeding values in mSec

    // Reset SPI
    SPIReset(GSPI_BASE);

    // enable FIFO
    SPIFIFOEnable(GSPI_BASE, SPI_TX_FIFO | SPI_RX_FIFO );

    // Configure SPI interface
    SPIConfigSetExpClk( GSPI_BASE, PRCMPeripheralClockGet(PRCM_GSPI), SPI_IF_BIT_RATE, SPI_MODE_MASTER, SPI_SUB_MODE_0, (SPI_SW_CTRL_CS|SPI_4PIN_MODE|SPI_TURBO_OFF| SPI_CS_ACTIVELOW|SPI_WL_8));

    // Enable SPI for communication
    SPIEnable(GSPI_BASE);

    motor::targetPosition = 500;

    while(1){
        delay(100);
        Report("motor::motorPos: %d\t targetPos: %d\t torque: %d \n\r", motor::motorPos, motor::targetPos, motor::torque);
    }
}

void PHOTO_IN_handler(){
    GPIOIntClear(GPIOA3_BASE, 0x1);
    motor::motorPos = 0; // reset motor::motorPos back to zero
}

void Encoder_I_Handler(){
    GPIOIntClear(GPIOA0_BASE, 0x80);
    uint8_t dI = GPIOPinRead(GPIOA0_BASE, 0x80);
    uint8_t dQ = GPIOPinRead(GPIOA1_BASE, 0x1);

    if(dI > I) /* rising edge */
        motor::motorPos += (Q == 0) ? 1 : -1; /* Q low  */
    else /* falling edge */
        motor::motorPos += (Q == 0) ? -1 : 1;

    I = dI;
    Q = dQ;
}

void Encoder_Q_Handler(){
    GPIOIntClear(GPIOA1_BASE, 0x1);
    uint8_t dI = GPIOPinRead(GPIOA0_BASE, 0x80);
    uint8_t dQ = GPIOPinRead(GPIOA1_BASE, 0x1);

    if(dQ > Q)   /* rising edge */
        motor::motorPos += (I == 0) ? -1 : 1; /* I low  */
    else /* falling edge */
        motor::motorPos += (I == 0) ? 1 : -1;

    I = dI;
    Q = dQ;
}

// ISR that is intended to execute every millisecond.
void TimerBaseIntHandler() {
    Timer_IF_InterruptClear(g_ulBase); // Clear the timer interrupt.
    millis++;
    motor::update();
}
