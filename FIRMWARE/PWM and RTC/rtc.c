/*
 * rtc.c
 *
 *  Created on: Nov 11, 2018
 *      Author: benja
 */

// Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//common includes
#include "rtc.h"
#include "i2c_if.h"
#include "myi2c.h"

//arrays(pointers) to store data to be sent on SDA
unsigned char RTCRead[20];
unsigned char RTCWrite[20];

/*
 * following registers used to store binary time data
 * index 0: years after 2000
 * 1: month of the year (1=Jan ... 12=Dec)
 * 2: date of the month (1-31)
 * 3: day of the week (0=Sun ... 6=Sat)
 * 4: hour of day (0-23)
 * 5: minute of hour (0-59)
 * 6: second of minute (0-59)
 */
unsigned char ConvertedTime[7];
unsigned char CurrentTime[7];
unsigned char RealTimeConv[8];

int RTCInit() {
    int i = 0;
    while(!(RTCRead[0] & 0x01)) {
        RTCWrite[0] = READ_STATUS_REG1;
        //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 1, 1);
        //I2C_IF_Read(RTC_ADDRESS, RTCRead, 1);

        MyI2CWrite(RTCWrite, 1, 1);
        MyI2CRead(RTCRead, 1);
    }
    if((RTCRead[0] & 0x01)) {
        for(i = 0; i <= 20000000; i++); //just a delay

        RTCWrite[0] = WRITE_STATUS_REG1;
        RTCWrite[1] = 0xA0;
        //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 2, 1);
        MyI2CWrite(RTCWrite, 2, 1);
        RTCWrite[0] = READ_STATUS_REG1;
        //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 1, 1);
        //I2C_IF_Read(RTC_ADDRESS, RTCRead, 1);
        MyI2CWrite(RTCWrite, 1, 1);
        MyI2CRead(RTCRead, 1);
        while(!(RTCRead[0] & 0x40)) {
            if(!(RTCRead[0] & 0x03)) {
                RTCWrite[0] = WRITE_STATUS_REG1;
                RTCWrite[1] = 0x70;
                //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 2, 1);
                MyI2CWrite(RTCWrite, 2, 1);
            }
            RTCWrite[0] = READ_STATUS_REG1;
            //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 1, 1);
            //I2C_IF_Read(RTC_ADDRESS, RTCRead, 1);
            MyI2CWrite(RTCWrite, 1, 1);
            MyI2CRead(RTCRead, 1);
        }
        return 1;
    }
    return 0;
}

int RTCSetTime() {
    int i = 0;
    unsigned char ConvTimeData = 0;
    RTCWrite[0] = WRITE_REALTIME_DATA1;

    for(i=0; i<7; i++) {
        ConvTimeData = 0;
        if(CurrentTime[i] >= 80) {
            ConvTimeData |= 0x80;
            CurrentTime[i] = CurrentTime[i] - 80;
        }
        if(i != 4) {
            if(CurrentTime[i] >= 40) {
                ConvTimeData |= 0x40;
                CurrentTime[i] = CurrentTime[i] - 40;
            }
        }
        if(CurrentTime[i] >= 20) {
            ConvTimeData |= 0x20;
            CurrentTime[i] = CurrentTime[i] - 20;
        }
        if(CurrentTime[i] >= 10) {
            ConvTimeData |= 0x10;
            CurrentTime[i] = CurrentTime[i] - 10;
        }
        if(CurrentTime[i] >= 8) {
            ConvTimeData |= 0x08;
            CurrentTime[i] = CurrentTime[i] - 8;
        }
        if(CurrentTime[i] >= 4) {
            ConvTimeData |= 0x04;
            CurrentTime[i] = CurrentTime[i] - 4;
        }
        if(CurrentTime[i] >= 2) {
            ConvTimeData |= 0x02;
            CurrentTime[i] = CurrentTime[i] - 2;
        }
        if(CurrentTime[i] >= 1) {
            ConvTimeData |= 0x01;
            CurrentTime[i] = CurrentTime[i] - 1;
        }
        RTCWrite[i+1] = ConvTimeData;
        RealTimeConv[i] = ConvTimeData;
    }
    //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 8, 1);
    MyI2CWrite(RTCWrite, 8, 1);        //norm
    return 1;
}

int RTCTimeGet() {
    RTCWrite[0] = READ_REALTIME_DATA1;
    //RTCWrite[0] = READ_INT1_REG;
    //I2C_IF_Write(RTC_ADDRESS, RTCWrite, 1, 1);
    //I2C_IF_Read(RTC_ADDRESS, RTCRead, 7);
    MyI2CWrite(RTCWrite, 1, 1);      //norm
    MyI2CRead(RTCRead, 7);
    return 1;
}

void RTCTimeConvert() {
    int i=0;
    unsigned char ConvTimeData = 0;

    //convert each byte
    for(i=0; i<7; i++) {
        //convert B7
        if((RTCRead[i] & 0x01)) {
            ConvTimeData = ConvTimeData + 1;
        }
        //convert B6
        if((RTCRead[i] & 0x02)) {
            ConvTimeData = ConvTimeData + 2;
        }
        //convert B5
        if((RTCRead[i] & 0x04)) {
            ConvTimeData = ConvTimeData + 4;
        }
        //convert B4
        if((RTCRead[i] & 0x08)) {
            ConvTimeData = ConvTimeData + 8;
        }
        //convert B3
        if((RTCRead[i] & 0x10)) {
            ConvTimeData = ConvTimeData + 10;
        }
        //convert B2
        if((RTCRead[i] & 0x20)) {
            ConvTimeData = ConvTimeData + 20;
        }
        //check if reading hours byte
        if(i!=4) {
            //convert B1
            if((RTCRead[i] & 0x40)) {
                ConvTimeData = ConvTimeData + 40;
            }
        }
        //convert B0
        if((RTCRead[i] & 0x80)) {
            ConvTimeData = ConvTimeData + 80;
        }
        ConvertedTime[i] = ConvTimeData;
        ConvTimeData = 0;
    }

}

//byte reverse method retrieved from: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void ArrayFlip(unsigned char array[], unsigned int arrayLength) {
    int i = 0;
    for (i = 1; i < arrayLength; i++) {
        array[i] = reverse(array[i]);
    }
}
