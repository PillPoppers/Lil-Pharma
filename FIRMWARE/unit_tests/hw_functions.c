/*
 * hw_functions.c
 *
 *  Created on: Dec 7, 2018
 *      Author: maxim
 */
#include "hw_functions.h"


void PinMuxConfig() {
    // Set unused pins to PIN_MODE_0 with the exception of JTAG pins 16,17,19,20
    // Enable Peripheral Clocks
    PRCMPeripheralClkEnable(PRCM_GPIOA0,  PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA1,  PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA2,  PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GPIOA3,  PRCM_RUN_MODE_CLK);

    PRCMPeripheralClkEnable(PRCM_ADC,     PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_I2CA0,   PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_GSPI,    PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_TIMERA1, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
    PRCMPeripheralClkEnable(PRCM_UARTA0,  PRCM_RUN_MODE_CLK);

    // initialize all touchscreen pins as inputs
    PinTypeGPIO(PIN_YD, PIN_MODE_0, false);
    GPIODirModeSet(PORT_YD, MASK_YD, GPIO_DIR_MODE_IN);
    PinTypeGPIO(PIN_YU, PIN_MODE_0, false);
    GPIODirModeSet(PORT_YU, MASK_YU, GPIO_DIR_MODE_IN);
    PinTypeGPIO(PIN_XR, PIN_MODE_0, false);
    GPIODirModeSet(PORT_XR, MASK_XR, GPIO_DIR_MODE_IN);
    PinTypeGPIO(PIN_XL, PIN_MODE_0, false);
    GPIODirModeSet(PORT_XL, MASK_XL, GPIO_DIR_MODE_IN);
    // touchscreen pins
    PinTypeGPIO(PIN_DC, PIN_MODE_0, false);
    GPIODirModeSet(PORT_DC, MASK_DC, GPIO_DIR_MODE_OUT);

//    PinTypeGPIO(PIN_16, PIN_MODE_0, false); // Configure PIN_16 for GPIO Output (JTAG_TDI, pin_12 on MOD, solenoid driver)
//    GPIODirModeSet(GPIOA2_BASE, 0x80, GPIO_DIR_MODE_OUT);
//    PinTypeGPIO(PIN_17, PIN_MODE_0, false); // Configure PIN_17 for GPIO Output (JTAG_TDO, pin_18 on MOD, for servo)
//    GPIODirModeSet(GPIOA3_BASE, 0x1, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_18, PIN_MODE_0, false); // Configure PIN_18 for GPIO Output (GPIO28, pin_19 on MOD, nSLEEP)
    GPIODirModeSet(GPIOA3_BASE, 0x10, GPIO_DIR_MODE_OUT); // TODO enable ISR on this pin and move it down with the ISR stuff.
    PinTypeGPIO(PIN_50, PIN_MODE_0, false); // Configure PIN_50 for GPIO Output (GPIO00, pin_44 on MOD, LCD_RESET)
    GPIODirModeSet(GPIOA0_BASE, 0x1, GPIO_DIR_MODE_OUT);

    PinTypeGPIO(PIN_60, PIN_MODE_0, false); // Configure PIN_60 for GPIO Output (GPIO05, pin_50 on MOD, MOTOR_IN_1)
    GPIODirModeSet(GPIOA0_BASE, 0x20, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_61, PIN_MODE_0, false); // Configure PIN_61 for GPIO Output (GPIO06, pin_51 on MOD, MOTOR_IN_2)
    GPIODirModeSet(GPIOA0_BASE, 0x40, GPIO_DIR_MODE_OUT);


//    TODO: verify this code is correct (the 0x40 may not be correct)
    PinTypeGPIO(PIN_53, PIN_MODE_0, false); // Configure PIN_53 for GPIO INT Input (GPIO30, pin_42 on MOD, PHOTO_IN)
    GPIOIntTypeSet(GPIOA3_BASE, 0x40, GPIO_BOTH_EDGES);
    GPIODirModeSet(GPIOA3_BASE, 0x40, GPIO_DIR_MODE_IN);
    GPIOIntRegister(GPIOA3_BASE, PHOTO_IN_handler);
    GPIOIntEnable(GPIOA3_BASE, 0x40);

    PinTypeGPIO(PIN_62, PIN_MODE_0, false); // Configure PIN_62 for GPIO INT Input (GPIO07, pin_53 on MOD, ENCODER_I)
    GPIODirModeSet(GPIOA0_BASE, 0x80, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(GPIOA0_BASE, 0x80, GPIO_BOTH_EDGES);
    GPIOIntRegister(GPIOA0_BASE, Encoder_I_Handler);
    GPIOIntEnable(GPIOA0_BASE, 0x80);

    PinTypeGPIO(PIN_63, PIN_MODE_0, false); // Configure PIN_63 for GPIO INT Input (GPIO08, pin_54 on MOD, ENCODER_Q)
    GPIODirModeSet(GPIOA1_BASE, 0x1, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(GPIOA1_BASE, 0x1, GPIO_BOTH_EDGES);
    GPIOIntRegister(GPIOA1_BASE, Encoder_Q_Handler);
    GPIOIntEnable(GPIOA1_BASE, 0x1);

    PinTypeSPI(PIN_08, PIN_MODE_7);   // Configure PIN_08 (pin 8 also for MOD) for SPI0 GSPI_CS
    PinTypeSPI(PIN_05, PIN_MODE_7);   // Configure PIN_05 (pin 5 also for MOD) for SPI0 GSPI_CLK
    PinTypeSPI(PIN_06, PIN_MODE_7);   // Configure PIN_06 (pin 6 also for MOD) for SPI0 GSPI_MISO
    PinTypeSPI(PIN_07, PIN_MODE_7);   // Configure PIN_07 (pin 7 also for MOD) for SPI0 GSPI_MOSI
    SPIReset(GSPI_BASE); // Reset SPI
//    SPIFIFOEnable(GSPI_BASE, SPI_TX_FIFO | SPI_RX_FIFO ); // enable FIFO
    SPIConfigSetExpClk(GSPI_BASE, PRCMPeripheralClockGet(PRCM_GSPI), SPI_IF_BIT_RATE, SPI_MODE_MASTER, SPI_SUB_MODE_0, (SPI_SW_CTRL_CS|SPI_4PIN_MODE|SPI_TURBO_OFF|SPI_CS_ACTIVELOW|SPI_WL_8)); // Configure SPI interface
    SPIEnable(GSPI_BASE); // Enable SPI for communication
    PRCMPeripheralReset(PRCM_GSPI); // Reset the peripheral

    PinTypeUART(PIN_55, PIN_MODE_3);  // Configure PIN_55 for UART0 UART0_TX
    PinTypeUART(PIN_57, PIN_MODE_3);  // Configure PIN_57 for UART0 UART0_RX

//    PinTypeI2C(PIN_01, PIN_MODE_1);   // Configure PIN_01 for I2C0 I2C_SCL
//    PinTypeI2C(PIN_02, PIN_MODE_1);   // Configure PIN_02 for I2C0 I2C_SDA

    PinTypeTimer(PIN_64, PIN_MODE_3); // Configure PIN_64 for TimerPWM5 GT_PWM05 (buzzer)
//    PinTypeTimer(PIN_21, PIN_MODE_9); // Configure PIN_21 for TimerPWM2 GT_PWM02 (LCD backlight) TODO: implement LCD backlight configurability

    // Initialize GPT A0 (in 32 bit mode) as periodic down counter.
    PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_TIMERA0);
    TimerConfigure(TIMERA0_BASE, TIMER_CFG_PERIODIC);
    TimerPrescaleSet(TIMERA0_BASE, TIMER_A, 0);
}

void init_millis_timer(){
    // configure millis timer
    Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0); // Configuring the timers
    Timer_IF_IntSetup(TIMERA0_BASE, TIMER_A, TimerBaseIntHandler); // Setup the interrupts for the timer timeouts.
    Timer_IF_Start(TIMERA0_BASE, TIMER_A, 1); // Turn on the timers feeding values in mSec
}

void InitTimerA0_SERVO() {
    TimerConfigure(TIMERA0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM)); //configure the timer mode
    TimerPrescaleSet(TIMERA0_BASE, TIMER_A, 25); //set prescale value: unit time = 0.3125 micro sec, max time = 20.48 ms
    TimerControlLevel(TIMERA0_BASE, TIMER_A , 1); //invert outputs if needed
    TimerLoadSet(TIMERA0_BASE, TIMER_A, SERVO_RELOAD_VALUE); //set reload value (signal HIGH)
    TimerMatchSet(TIMERA0_BASE, TIMER_A, 65535); ///set match value (signal LOW) fix last parameter here. max is 65535. Set to same as CloseDoor
    TimerPrescaleMatchSet(TIMERA0_BASE, TIMER_A, 1);
    TimerEnable(TIMERA0_BASE, TIMER_A); //enable timer
}

void Open_door() {
    TimerDisable(TIMERA0_BASE, TIMER_A);
    TimerMatchSet(TIMERA0_BASE, TIMER_A, 65535); //TODO: fix last parameter here. max is 65535
    TimerPrescaleMatchSet(TIMERA0_BASE, TIMER_A, 2);
    TimerEnable(TIMERA0_BASE, TIMER_A);
}

void Close_door() {
    TimerDisable(TIMERA0_BASE, TIMER_A);
    TimerMatchSet(TIMERA0_BASE, TIMER_A, 65535); //TODO: fix last parameter here. 65535 is max
    TimerPrescaleMatchSet(TIMERA0_BASE, TIMER_A, 1);
    TimerEnable(TIMERA0_BASE, TIMER_A);
}

void InitTimer2B_BUZZER() {
    TimerConfigure(TIMERA2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM)); //configure the timer mode
    TimerPrescaleSet(TIMERA2_BASE, TIMER_B, 0); //set prescale value: unit time = 0.3125 micro sec, max time = 20.48 ms TODO: adjust this for right frequency
    TimerControlLevel(TIMERA2_BASE, TIMER_B , 1); //invert outputs if needed
    //TODO: adjust for right frequency
    TimerLoadSet(TIMERA2_BASE, TIMER_B, BUZZER_RELOAD_VALUE); //set reload value (signal HIGH)
    //TODO: adjust for right frequency
    TimerMatchSet(TIMERA2_BASE, TIMER_B, (BUZZER_RELOAD_VALUE / 2)); //set match value (signal LOW)
    TimerPrescaleMatchSet(TIMERA2_BASE, TIMER_B, 0); //adjust for right frequency
}

void buzz_enable(){
    //enable timer
     TimerEnable(TIMERA2_BASE, TIMER_B);//enable timer
}
void buzz_disable(){
    TimerDisable(TIMERA2_BASE, TIMER_B);
}


