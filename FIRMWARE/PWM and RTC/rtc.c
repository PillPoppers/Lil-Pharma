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
#include "myi2c.h"

//arrays(pointers) to store data to be sent on SDA
unsigned char RTCRead[];
unsigned char RTCWrite[];

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

int RTCInit() {
    int i=0;
    Myi2cInit();
    Myi2cSlaveInit();
    RTCWrite[0] = READ_STATUS_REG1;
    Myi2cWrite(&RTCWrite, 1, 1);
    Myi2cRead(&RTCRead, 1);
    if((RTCRead[0] & 0x01)) {
        for(i = 0; i <= 40000000; i++);
        RTCWrite[0] = WRITE_STATUS_REG1;
        RTCWrite[1] = 0x80;
        Myi2cWrite(&RTCWrite, 2, 1);
        RTCWrite[0] = READ_STATUS_REG1;
        Myi2cWrite(&RTCWrite, 1, 1);
        Myi2cRead(&RTCRead, 1);
        if((RTCRead[0] & 0x01)) {
            RTCWrite[0] = WRITE_STATUS_REG1;
            RTCWrite[1] = 0x40;
            Myi2cWrite(&RTCWrite, 2, 1);
            return 1;
        }
    }
    return 0;
}

int RTCSetTime() {
    int i = 0;
    unsigned char ConvTimeData = 0;
    RTCWrite[0] = WRITE_REALTIME_DATA1;

    for(i=0; i<7; i++) {
        if(CurrentTime[i] > 80) {
            ConvTimeData |= 0x80;
            CurrentTime[i] = CurrentTime[i] - 80;
        }
        if(i != 4) {
            if(CurrentTime[i] > 40) {
                ConvTimeData |= 0x40;
                CurrentTime[i] = CurrentTime[i] - 40;
            }
        }
        if(CurrentTime[i] > 20) {
            ConvTimeData |= 0x20;
            CurrentTime[i] = CurrentTime[i] - 20;
        }
        if(CurrentTime[i] > 10) {
            ConvTimeData |= 0x10;
            CurrentTime[i] = CurrentTime[i] - 10;
        }
        if(CurrentTime[i] > 8) {
            ConvTimeData |= 0x08;
            CurrentTime[i] = CurrentTime[i] - 8;
        }
        if(CurrentTime[i] > 4) {
            ConvTimeData |= 0x04;
            CurrentTime[i] = CurrentTime[i] - 4;
        }
        if(CurrentTime[i] > 2) {
            ConvTimeData |= 0x02;
            CurrentTime[i] = CurrentTime[i] - 2;
        }
        if(CurrentTime[i] > 1) {
            ConvTimeData |= 0x01;
            CurrentTime[i] = CurrentTime[i] - 1;
        }
        RTCWrite[i+1] = ConvTimeData;
    }
    Myi2cWrite(&RTCWrite, 8, 1);
    return 0;
}

int RTCTimeGet() {
    RTCWrite[0] = READ_REALTIME_DATA1;
    Myi2cWrite(&RTCWrite, 1, 1);
    Myi2cRead(&RTCRead, 7);
    RTCTimeConvert();
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

