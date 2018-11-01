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

// Common interface includes
#include "arduino.h"
#include "uart_if.h"
#include "pinMux.h"
#include "timer_if.h"

// modified arduino / adafruit library:
//#include "Adafruit_GFX.h"
//#include "Adafruit_HX8357.h"






//*****************************************************************************
// GLOBAL VARIABLES -- Start
//*****************************************************************************
static uint8_t g_ucTxBuff[64];
static uint8_t g_ucRxBuff[64];
static uint8_t ucTxBuffNdx;
static uint8_t ucRxBuffNdx;

#define MASTER_MODE      0
#define SPI_IF_BIT_RATE  100000
#define TR_BUFF_SIZE     100

#define UART_BAUD_RATE 115200


static volatile unsigned long g_ulBase;


#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif


// Adafruit_HX8357 tft = Adafruit_HX8357();

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
/*
//unsigned long testFillScreen() {
////    unsigned long start = micros();
//    tft.fillScreen(HX8357_RED);
//    tft.fillScreen(HX8357_GREEN);
//    tft.fillScreen(HX8357_BLUE);
//    tft.fillScreen(HX8357_WHITE);
//    return 0;//micros() - start;
//}
//
//unsigned long testText() {
//    tft.fillScreen(HX8357_BLACK);
////    unsigned long start = micros();
//    tft.setCursor(0, 0);
//    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(1);
////    tft.println("Hello World!");
//    tft.setTextColor(HX8357_YELLOW); tft.setTextSize(2);
////    tft.println(1234.56);
//    tft.setTextColor(HX8357_RED);    tft.setTextSize(3);
////    tft.println(0xDEADBEEF, HEX);
////    tft.println();
//    tft.setTextColor(HX8357_GREEN);
//    tft.setTextSize(5);
////    tft.println("Groop");
//    tft.setTextSize(2);
////    tft.println("I implore thee,");
//    tft.setTextSize(1);
////    tft.println("my foonting turlingdromes.");
////    tft.println("And hooptiously drangle me");
////    tft.println("with crinkly bindlewurdles,");
////    tft.println("Or I will rend thee");
////    tft.println("in the gobberwarts");
////    tft.println("with my blurglecruncheon,");
////    tft.println("see if I don't!");
//    tft.setTextColor(HX8357_WHITE);
////    tft.println(F("Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister was reading, but it had no pictures or conversations in it, 'and what is the use of a book,' thought Alice 'without pictures or conversations?'"));
////    tft.println(F("So she was considering in her own mind (as well as she could, for the hot day made her feel very sleepy and stupid), whether the pleasure of making a daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly a White Rabbit with pink eyes ran close by her."));
////    tft.println(F("There was nothing so very remarkable in that; nor did Alice think it so very much out of the way to hear the Rabbit say to itself, 'Oh dear! Oh dear! I shall be late!' (when she thought it over afterwards, it occurred to her that she ought to have wondered at this, but at the time it all seemed quite natural); but when the Rabbit actually took a watch out of its waistcoat-pocket, and looked at it, and then hurried on, Alice started to her feet, for it flashed across her mind that she had never before seen a rabbit with either a waistcoat-pocket, or a watch to take out of it, and burning with curiosity, she ran across the field after it, and fortunately was just in time to see it pop down a large rabbit-hole under the hedge."));
////    tft.println(F("In another moment down went Alice after it, never once considering how in the world she was to get out again."));
////    tft.println(F("The rabbit-hole went straight on like a tunnel for some way, and then dipped suddenly down, so suddenly that Alice had not a moment to think about stopping herself before she found herself falling down a very deep well."));
////    tft.println(F("Either the well was very deep, or she fell very slowly, for she had plenty of time as she went down to look about her and to wonder what was going to happen next. First, she tried to look down and make out what she was coming to, but it was too dark to see anything; then she looked at the sides of the well, and noticed that they were filled with cupboards and book-shelves; here and there she saw maps and pictures hung upon pegs. She took down a jar from one of the shelves as she passed; it was labelled 'ORANGE MARMALADE', but to her great disappointment it was empty: she did not like to drop the jar for fear of killing somebody, so managed to put it into one of the cupboards as she fell past it."));
//  return 0;//micros() - start;
//}
//
//unsigned long testLines(uint16_t color) {
////    unsigned long start, t;
//    int           x1, y1, x2, y2,
//                w = tft.width(),
//                h = tft.height();
//
//    tft.fillScreen(HX8357_BLACK);
//
//    x1 = y1 = 0;
//    y2    = h - 1;
////    start = micros();
//    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
//    x2    = w - 1;
//    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
////    t     = micros() - start; // fillScreen doesn't count against timing
//
//
//    return 0;//micros() - start;
//}
//
//unsigned long testFastLines(uint16_t color1, uint16_t color2) {
////  unsigned long start;
//  int           x, y, w = tft.width(), h = tft.height();
//
//  tft.fillScreen(HX8357_BLACK);
////  start = micros();
//  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
//  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);
//
//  return 0;// micros() - start;
//}
//
//unsigned long testRects(uint16_t color) {
////  unsigned long start;
//  int           n, i, i2,
//                cx = tft.width()  / 2,
//                cy = tft.height() / 2;
//
//  tft.fillScreen(HX8357_BLACK);
//  n     = min(tft.width(), tft.height());
////  start = micros();
//  for(i=2; i<n; i+=6) {
//    i2 = i / 2;
//    tft.drawRect(cx-i2, cy-i2, i, i, color);
//  }
//
//  return 0;//micros() - start;
//}
//
//unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
//  unsigned long start, t = 0;
//  int           n, i, i2,
//                cx = tft.width()  / 2 - 1,
//                cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(HX8357_BLACK);
//  n = min(tft.width(), tft.height());
//  for(i=n; i>0; i-=6) {
//    i2    = i / 2;
////    start = micros();
//    tft.fillRect(cx-i2, cy-i2, i, i, color1);
////    t    += micros() - start;
//    // Outlines are not included in timing results
//    tft.drawRect(cx-i2, cy-i2, i, i, color2);
//  }
//
//  return t;
//}
//
//unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
//  unsigned long start;
//  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;
//
//  tft.fillScreen(HX8357_BLACK);
////  start = micros();
//  for(x=radius; x<w; x+=r2) {
//    for(y=radius; y<h; y+=r2) {
//      tft.fillCircle(x, y, radius, color);
//    }
//  }
//
//  return 0;//micros() - start;
//}
//
//unsigned long testCircles(uint8_t radius, uint16_t color) {
//  unsigned long start;
//  int           x, y, r2 = radius * 2,
//                w = tft.width()  + radius,
//                h = tft.height() + radius;
//
//  // Screen is not cleared for this one -- this is
//  // intentional and does not affect the reported time.
////  start = micros();
//  for(x=0; x<w; x+=r2) {
//    for(y=0; y<h; y+=r2) {
//      tft.drawCircle(x, y, radius, color);
//    }
//  }
//
//  return 0;//micros() - start;
//}
//
//unsigned long testTriangles() {
//  unsigned long start;
//  int           n, i, cx = tft.width()  / 2 - 1,
//                      cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(HX8357_BLACK);
//  n     = min(cx, cy);
////  start = micros();
//  for(i=0; i<n; i+=5) {
//    tft.drawTriangle(
//      cx    , cy - i, // peak
//      cx - i, cy + i, // bottom left
//      cx + i, cy + i, // bottom right
//      tft.color565(200, 20, i));
//  }
//
//  return 0;//micros() - start;
//}
//
//unsigned long testFilledTriangles() {
//  unsigned long start, t = 0;
//  int           i, cx = tft.width()  / 2 - 1,
//                   cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(HX8357_BLACK);
////  start = micros();
//  for(i=min(cx,cy); i>10; i-=5) {
////    start = micros();
//    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
//      tft.color565(0, i, i));
////    t += micros() - start;
//    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
//      tft.color565(i, i, 0));
//  }
//
//  return t;
//}
//
//unsigned long testRoundRects() {
//  unsigned long start;
//  int           w, i, i2,
//                cx = tft.width()  / 2 ,
//                cy = tft.height() / 2 ;
//
//  tft.fillScreen(HX8357_BLACK);
//  w     = min(tft.width(), tft.height());
////  start = micros();
//  for(i=0; i<w; i+=8) {
//    i2 = i / 2 - 2;
//    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 100, 100));
//  }
//
//  return 0;// micros() - start;
//}
//
//unsigned long testFilledRoundRects() {
//  unsigned long start;
//  int           i, i2,
//                cx = tft.width()  / 2 + 10,
//                cy = tft.height() / 2 + 10;
//
//  tft.fillScreen(HX8357_BLACK);
////  start = micros();
//  for(i=min(tft.width(), tft.height()) - 20; i>25; i-=6) {
//    i2 = i / 2;
//    tft.fillRoundRect(cx-i2, cy-i2, i-20, i-20, i/8, tft.color565(100, i/2, 100));
//  }

  return 0;//micros() - start;
}
*/


// ISR that is intended to execute every 500 milliseconds.
void TimerBaseIntHandler(void) {
    Timer_IF_InterruptClear(g_ulBase); // Clear the timer interrupt.
    millis++;
}


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
    PinTypeGPIO(PIN_16, PIN_MODE_0, false); // Configure PIN_16 for GPIO Output (JTAG_TDI, pin_12 on MOD, solenoid driver)
    GPIODirModeSet(GPIOA2_BASE, 0x80, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_17, PIN_MODE_0, false); // Configure PIN_17 for GPIO Output (JTAG_TDO, pin_18 on MOD, currently unused (?))
    GPIODirModeSet(GPIOA3_BASE, 0x1, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_18, PIN_MODE_0, false); // Configure PIN_18 for GPIO Output (GPIO28, pin_19 on MOD, nSLEEP)
    GPIODirModeSet(GPIOA3_BASE, 0x10, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_50, PIN_MODE_0, false); // Configure PIN_50 for GPIO Output (GPIO00, pin_44 on MOD, LCD_RESET)
    GPIODirModeSet(GPIOA0_BASE, 0x1, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_60, PIN_MODE_0, false); // Configure PIN_60 for GPIO Output (GPIO05, pin_50 on MOD, MOTOR_IN_1)
    GPIODirModeSet(GPIOA0_BASE, 0x20, GPIO_DIR_MODE_OUT);
    PinTypeGPIO(PIN_61, PIN_MODE_0, false); // Configure PIN_61 for GPIO Output (GPIO06, pin_51 on MOD, MOTOR_IN_2)
    GPIODirModeSet(GPIOA0_BASE, 0x40, GPIO_DIR_MODE_OUT);

    PinTypeGPIO(PIN_53, PIN_MODE_0, false); // Configure PIN_53 for GPIO Input (GPIO30, pin_42 on MOD, PHOTO_IN)
    GPIODirModeSet(GPIOA3_BASE, 0x40, GPIO_DIR_MODE_IN);
    PinTypeGPIO(PIN_62, PIN_MODE_0, false); // Configure PIN_62 for GPIO Input (GPIO07, pin_52 on MOD, ENCODER_I)
    GPIODirModeSet(GPIOA0_BASE, 0x80, GPIO_DIR_MODE_IN);
    PinTypeGPIO(PIN_63, PIN_MODE_0, false); // Configure PIN_63 for GPIO Input (GPIO08, pin_53 on MOD, ENCODER_Q)
    GPIODirModeSet(GPIOA1_BASE, 0x1, GPIO_DIR_MODE_IN);

    PinTypeI2C(PIN_01, PIN_MODE_1); // Configure PIN_01 for I2C0 I2C_SCL
    PinTypeI2C(PIN_02, PIN_MODE_1); // Configure PIN_02 for I2C0 I2C_SDA

    PinTypeSPI(PIN_08, PIN_MODE_7); // Configure PIN_08 (pin 8 also for MOD) for SPI0 GSPI_CS
    PinTypeSPI(PIN_05, PIN_MODE_7); // Configure PIN_05 (pin 5 also for MOD) for SPI0 GSPI_CLK
    PinTypeSPI(PIN_06, PIN_MODE_7); // Configure PIN_06 (pin 6 also for MOD) for SPI0 GSPI_MISO
    PinTypeSPI(PIN_07, PIN_MODE_7); // Configure PIN_07 (pin 7 also for MOD) for SPI0 GSPI_MOSI

    PinTypeUART(PIN_55, PIN_MODE_3); // Configure PIN_55 for UART0 UART0_TX
    PinTypeUART(PIN_57, PIN_MODE_3); // Configure PIN_57 for UART0 UART0_RX

    PinTypeTimer(PIN_64, PIN_MODE_3); // Configure PIN_64 for TimerPWM5 GT_PWM05 (buzzer)
    PinTypeTimer(PIN_21, PIN_MODE_9); // Configure PIN_21 for TimerPWM2 GT_PWM02 (LCD backlight)



    // Initialize GPT A0 (in 32 bit mode) as periodic down counter.
    PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_TIMERA0);
    TimerConfigure(TIMERA0_BASE,TIMER_CFG_PERIODIC);
    TimerPrescaleSet(TIMERA0_BASE,TIMER_A, 0);


    PinTypeADC(PIN_58, PIN_MODE_255); // Configure PIN_58 for ADC0 ADC_CH1 (LCD touchscreen)
    PinTypeADC(PIN_59, PIN_MODE_255); // Configure PIN_59 for ADC0 ADC_CH2 (LCD touchscreen)
}

void setup() {

    MAP_PRCMPeripheralReset(PRCM_GSPI); // Reset the peripheral
    BoardInit(); // Initialize Board configurations
    PinMuxConfig(); // Muxing UART and SPI lines.
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK); // Enable the SPI module clock
    Message("HX8357D Test!\n");

    /*
    tft.begin(HX8357D);

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
//    Message("Display Power Mode: 0x"); Serial.println(x, HEX);
//    x = tft.readcommand8(HX8357_RDMADCTL);
//    Message("MADCTL Mode: 0x"); Serial.println(x, HEX);
//    x = tft.readcommand8(HX8357_RDCOLMOD);
//    Message("Pixel Format: 0x"); Serial.println(x, HEX);
//    x = tft.readcommand8(HX8357_RDDIM);
//    Message("Image Format: 0x"); Serial.println(x, HEX);
//    x = tft.readcommand8(HX8357_RDDSDR);
//    Message("Self Diagnostic: 0x"); Serial.println(x, HEX);

    Message("Benchmark                Time (microseconds)\n");

    tft.setRotation(1);

    Message("Text                     \n");
    testText();
    delay(500);

    Message("Lines                    ");
    testLines(HX8357_CYAN);
    delay(500);

    Message("Rectangles (outline)     \n");
    testRects(HX8357_GREEN);
    delay(500);

    tft.fillScreen(HX8357_BLACK);
    Message("Circles (outline)        \n");
    testCircles(10, HX8357_RED);
    delay(500);


    Message("Triangles (outline)      \n");
    testTriangles();
    delay(500);

    Message("Triangles (filled)       \n");
    testFilledTriangles();
    delay(500);


    Message("Rounded rects (outline)  \n");
    testRoundRects();
    delay(500);

    Message("Rounded rects (filled)   \n");
    testFilledRoundRects();
    delay(500);

    Message("Done!\n");
    */
}

uint8_t a = 0;
uint8_t dummy = 0;
void loop(void) {




    //SPITransfer (GSPI_BASE, &a, &dummy, 1, 1);

    // stupid test that should not stay in final codebase
    if (millis > 1000){
        //Message("hello world");
        millis = 0;
    }

    /*
    for(uint8_t rotation=0; rotation<4; rotation++) {
        tft.setRotation(rotation);
        testText();
        delay(1000);
    }
    */
}


void main() {
    // conigure UART interface:
    MAP_UARTConfigSetExpClk(CONSOLE,
                            MAP_PRCMPeripheralClockGet(CONSOLE_PERIPH),
                            UART_BAUD_RATE,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)
    );

//TODO: cases really nasty system hang and crashes computer. This means it's a simple problem that just needs fixing.
// Configure SPI interface
//    SPIConfigSetExpClk(
//            GSPI_BASE,
//            PRCMPeripheralClockGet(PRCM_GSPI),
//            SPI_IF_BIT_RATE,
//            SPI_MODE_MASTER,
//            SPI_SUB_MODE_0,
//            (SPI_SW_CTRL_CS | SPI_4PIN_MODE | SPI_TURBO_OFF | SPI_CS_ACTIVEHIGH | SPI_WL_8)
//    );
//    SPIEnable(GSPI_BASE);


    g_ulBase = TIMERA0_BASE; // Base address for first timer
    Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0); // Configuring the timers
    Timer_IF_IntSetup(g_ulBase, TIMER_A, TimerBaseIntHandler); // Setup the interrupts for the timer timeouts.
    Timer_IF_Start(g_ulBase, TIMER_A, 500); // Turn on the timers feeding values in mSec

    // "arduino" style coding.
    setup();

    while(1){

        loop();
    }
}

