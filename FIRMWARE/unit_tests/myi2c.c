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

void MyI2CInit() {
    //enable peripheral clock
    PRCMPeripheralClkEnable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
    PRCMPeripheralReset(PRCM_I2CA0);
    //set speed of clock
    I2CMasterInitExpClk(I2C_BASE , CLOCK_SPEED, 0);
}

void MyI2CDisable(void) {
    //disable peripheral clock
    PRCMPeripheralClkDisable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
}

//initiate command sequence for I2C master block
//commands found in i2c.h file in driverlib
void MyI2CCommand(unsigned long command) {
    //clear interrupts
    I2CMasterIntClearEx(I2C_BASE, I2CMasterIntStatusEx(I2C_BASE,false));

    //set timeout value
    I2CMasterTimeoutSet(I2C_BASE, I2C_TIMEOUT_VAL);

    //send command
    I2CMasterControl(I2C_BASE, command);

    //check interrupts for completion of command
    while((I2CMasterIntStatusEx(I2C_BASE, false) & (I2C_INT_MASTER | I2C_MRIS_CLKTOUT)) == 0) {
    }
}

//writes to RTC, 1 byte at a time
//StopBit usually 1
void MyI2CWrite(unsigned char *DataPt, unsigned char DataLength, unsigned char StopBit)  {
    //set slave address and type of communication
    I2CMasterSlaveAddrSet(I2C_BASE, RTC_ADDRESS, false);

    //write first byte to FIFO
    I2CMasterDataPut(I2C_BASE, *DataPt);

    //start transfer
    MyI2CCommand(I2C_MASTER_CMD_BURST_SEND_START);

    DataLength--;
    DataPt++;

    while(DataLength) {
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

    //receive byte if initial length = 1
    *DataPt = I2CMasterDataGet(I2C_BASE);
}
