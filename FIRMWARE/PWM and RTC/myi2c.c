/*
 * rtc.c
 */

// Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "i2c.h"
#include "pin.h"
#include "prcm.h"

// project includes
#include "myi2c.h"

#define DELAY       20

void MyI2CInit() {
    //enable peripheral clock
    PRCMPeripheralClkEnable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_I2CA0);
    //set speed of clock
    I2CMasterInitExpClk(I2C_BASE , CLOCK_SPEED, 0);
    //I2CMasterIntEnable(I2C_BASE);
    //I2CMasterIntEnableEx(I2C_BASE, I2C_MASTER_INT_TIMEOUT);
}

void MyI2CDisable(void) {
    //disable peripheral clock
    PRCMPeripheralClkDisable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
}

//initiate command sequence for I2C master block
//commands found in i2c.h file in driverlib
int MyI2CCommand(unsigned long command) {
    //clear interrupts
    I2CMasterIntClearEx(I2C_BASE, I2CMasterIntStatusEx(I2C_BASE,false));

    //set timeout value
    I2CMasterTimeoutSet(I2C_BASE, I2C_TIMEOUT_VAL);

    //send command
    I2CMasterControl(I2C_BASE, command);

    //check interrupts for completion of command
    while((I2CMasterIntStatusEx(I2C_BASE, false) & (I2C_INT_MASTER | I2C_MRIS_CLKTOUT)) == 0) {
    }

    //
    /*    // Check for any errors in transfer
        //
        if(MAP_I2CMasterErr(I2C_BASE) != I2C_MASTER_ERR_NONE)
        {
            switch(ulCmd)
            {
            case I2C_MASTER_CMD_BURST_SEND_START:
            case I2C_MASTER_CMD_BURST_SEND_CONT:
            case I2C_MASTER_CMD_BURST_SEND_STOP:
                I2CMasterControl(I2C_BASE,
                             I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
                break;
            case I2C_MASTER_CMD_BURST_RECEIVE_START:
            case I2C_MASTER_CMD_BURST_RECEIVE_CONT:
            case I2C_MASTER_CMD_BURST_RECEIVE_FINISH:
                I2CMasterControl(I2C_BASE,
                             I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
                break;
            default:
                break;
            }
            return 0;
        }

        return 1;
    }*/
    return 1;
}

//writes to RTC, 1 byte at a time
//StopBit usually 1
void MyI2CWrite(unsigned char *DataPt, unsigned char DataLength, unsigned char StopBit)  {
    int i = 0;

    //set slave address and type of communication
    I2CMasterSlaveAddrSet(I2C_BASE, RTC_ADDRESS, false);

    //write first byte to FIFO
    I2CMasterDataPut(I2C_BASE, *DataPt);

    //start transfer
    MyI2CCommand(I2C_MASTER_CMD_BURST_SEND_START);

    DataLength--;
    DataPt++;

    while(DataLength) {
        // add short delay
        for(i = 0; i < DELAY; i++);

        //write next byte to FIFO
        I2CMasterDataPut(I2C_BASE, *DataPt);

        //continue transfer
        MyI2CCommand(I2C_MASTER_CMD_BURST_SEND_CONT);

        DataLength--;
        DataPt++;
    }

    //send StopBit if necessary
    if(StopBit) {
        MyI2CCommand(I2C_MASTER_CMD_BURST_SEND_STOP);
    }
}


void MyI2CRead(unsigned char *DataPt, unsigned char DataLength) {
    int i = 0;

    //set slave address and type of communication
    I2CMasterSlaveAddrSet(I2C_BASE, RTC_ADDRESS, true);

    // if single byte data
    if(DataLength == 1) {
        MyI2CCommand(I2C_MASTER_CMD_SINGLE_RECEIVE);
    }
    else {
        MyI2CCommand(I2C_MASTER_CMD_BURST_RECEIVE_START);
    }

    DataLength--;

    //if initial DataLength > 1
    while(DataLength) {
        // add short delay
        for(i = 0; i < DELAY; i++);

        //receive byte
        *DataPt = I2CMasterDataGet(I2C_BASE);

        //adjust DataPt and DataLength
        DataLength--;
        DataPt++;

        if(DataLength) {
            MyI2CCommand(I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        }
        else {
            MyI2CCommand(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        }
    }
    // add short delay
    for(i = 0; i < DELAY; i++);

    //receive byte if initial length = 1
    *DataPt = I2CMasterDataGet(I2C_BASE);
}





/*
//TI example for write
void MyI2CWrite(unsigned char ucDevAddr,
            unsigned char *ucBuffer,
            unsigned long ulSize)
{
    unsigned long ulNdx;

    // Set I2C codec slave address
    I2CMasterSlaveAddrSet(I2CA0_BASE,ucDevAddr, false);

    // Write the first byte to the controller.
    I2CMasterDataPut(I2CA0_BASE, ucBuffer[0]);
    I2CMasterIntClearEx(I2CA0_BASE, I2C_INT_MASTER);

    if( ulSize == 1)
    {
        I2CMasterControl(I2CA0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    }
    else
    {
        // Continue the transfer.
        I2CMasterControl(I2CA0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until the current byte has been transferred.
        while((I2CMasterIntStatusEx(I2CA0_BASE, false) & I2C_INT_MASTER) == 0)
        {
        }

        for(ulNdx = 1; ulNdx < ulSize-1; ulNdx++)
        {
            // Write the next byte to the controller.
            I2CMasterDataPut(I2CA0_BASE,ucBuffer[ulNdx]);

            // Clear Master Interrupt
            I2CMasterIntClearEx(I2CA0_BASE, I2C_INT_MASTER);

            // Continue the transfer.
            I2CMasterControl(I2CA0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until the current byte has been transferred.
            while((I2CMasterIntStatusEx(I2CA0_BASE, false) & I2C_INT_MASTER) == 0)
            {
            }
        }

        // Write the last byte to the controller.
        I2CMasterDataPut(I2CA0_BASE, ucBuffer[ulSize-1]);
        I2CMasterIntClearEx(I2CA0_BASE, I2C_INT_MASTER);

        // End the transfer.
        I2CMasterControl(I2CA0_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);
    }

    // Wait until the current byte has been transferred.
    while((I2CMasterIntStatusEx(I2CA0_BASE, false) & I2C_INT_MASTER) == 0)
    {
    }

}
*/
