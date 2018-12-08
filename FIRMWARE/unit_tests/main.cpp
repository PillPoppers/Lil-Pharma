extern "C" {
#include "hw_functions.h"
#include "touch.h"
}
//TODO: convert "legacy C" code into C++ friendly code.
#include "motor.h"
#include "LCD.h"
#include "graphicsClasses.h"


volatile uint32_t millis = 0;
volatile uint8_t I, Q;

static void BoardInit(void) {

    // In case of TI-RTOS vector table is initialize by OS itself
    #ifndef USE_TIRTOS
      // Set vector table base
    #if defined(ccs)
        IntVTableBaseSet((uint32_t)&g_pfnVectors[0]);
    #endif
    #if defined(ewarm)
        IntVTableBaseSet((uint32_t)&__vector_table);
    #endif
    #endif
        // Enable Processor
        MAP_IntMasterEnable();
        MAP_IntEnable(FAULT_SYSTICK);
        PRCMCC3200MCUInit();
}

void main() {
    BoardInit(); // Initialize Board configurations
    PinMuxConfig(); // Muxing UART and SPI lines.

    InitTerm(); // initialize  the Terminal.
    ClearTerm(); // Clearing the Terminal.

    InitTimer2B_BUZZER();
    init_millis_timer();

    motor::targetPos = 0; // set target position to 0 initially.
    motor::posCtrlEn = 0; // Disable positional controller.
    motor::stop();

    while(1){
        print("THIS IS A TEST OF ALL HARDWARE. --Maxim Poe 12/8/2018\r\n\r\n\r\n");
        delay(500);

        print("Buzzer test: Buzzer should audibly chirp.");
        delay(500);
        for(int i = 0; i < 10; i++){
            print(". ");
            delay(250);
            buzz_enable();
            delay(10);
            buzz_disable();
            delay(10);
        }
        print("\r\nBuzzer test Complete!\r\n");
        delay(250);


        print("Touch screen test. Feel free to press the touch screen and observe values printed. You have 10 seconds.\r\n");
        delay(500);

        int x = 0, y = 0;
        for(int i = 0; i < 250; i++){
            delay(100);
            x = readX();
            y = readY();
            Report("touch X,Y: %d, %d\r\n", x, y); // TODO: figure out macro printf definition in defines.h
        }
        print("Touch screen test Complete!\r\n");
        delay(250);

        print("Motor test. This test will rotate the motor using low-level functions then enable positional controller to test control-system response.\r\n");

        print("Motor test 1/2. (forward/stop/backward...) /r/n");
        motor::forward();
        delay(2000);
        motor::stop();
        delay(2000);
        motor::backward();
        delay(2000);
        motor::stop();
        print("Motor test 2/2: positional control. Please enter position. Press \"q\" to exit. \r\n");
        print("ERR. not implemented. \r\n"); //#TODO
        delay(1000);

        print("LCD test: \r\n");
        delay(500);



        for(int i = 0; i < 100; ++i){
            InitializeLCD();
            delay(100);
            fillScreen(WHITE);
            fillScreen(BLACK);
        }
        print("LCD test complete!\r\n");
        print("TODO: test solonoid and servo.\r\c"); //TODO
    }
}

void PHOTO_IN_handler(){
    GPIOIntClear(GPIOA3_BASE, 0x1);
    motor::motorPos = 0; // reset motor::motorPos back to zero
}

void Encoder_I_Handler(){
    GPIOIntClear(GPIOA0_BASE, 0x80);
    uint8_t dI = GPIOPinRead(GPIOA0_BASE, 0x80) ? 1:0;
    uint8_t dQ = GPIOPinRead(GPIOA1_BASE, 0x01) ? 1:0;

    if(dI > I) /* rising edge */
        motor::motorPos += (Q == 0) ? 1 : -1; /* Q low  */
    else /* falling edge */
        motor::motorPos += (Q == 0) ? -1 : 1;

    I = dI;
    Q = dQ;
}

void Encoder_Q_Handler(){
    GPIOIntClear(GPIOA1_BASE, 0x1);
    uint8_t dI = GPIOPinRead(GPIOA0_BASE, 0x80) ? 1:0;
    uint8_t dQ = GPIOPinRead(GPIOA1_BASE, 0x01) ? 1:0;

    if(dQ > Q)   /* rising edge */
        motor::motorPos += (I == 0) ? -1 : 1; /* I low  */
    else /* falling edge */
        motor::motorPos += (I == 0) ? 1 : -1;

    I = dI;
    Q = dQ;
}


// ISR that is intended to execute every millisecond.
void TimerBaseIntHandler() {
    Timer_IF_InterruptClear(TIMERA0_BASE); // Clear the timer interrupt.
    millis++;

    if(motor::posCtrlEn){
        motor::update();
    }
}
