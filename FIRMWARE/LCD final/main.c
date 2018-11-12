// Driverlib includes
#include "hw_types.h"

// Custom libraries
#include "lcd.h"
#include "util.h"




static void
BoardInit(void)
{
    PRCMCC3200MCUInit();
}


void main()
{
    // Initialize Board configurations
    BoardInit();

    // Mind the order: 1) PIN 2) LCDComm 3) LCD (this is dealt by nested calls)

    // Dependent on
    SPIPinMuxConfig();
    InitializeLCDCommunication();
    InitializeLCD();

    //fillRect(0,0,310, 470, RED);
    char array[] = "Blue";
    writeText(50, 50, 10, array, BLACK, WHITE);

    while(1)
    {
     //   writeCommand(DISPOFF);
        //writeParameter(0x07);
        SetDC();
       UtilsDelay(1000);
       ResetDC();
       UtilsDelay(1000);


    }

}

