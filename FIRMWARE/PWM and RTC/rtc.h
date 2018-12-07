/*
 * rtc.h
 *
 *  Created on: Nov 11, 2018
 *      Author: benja
 */

#ifndef RTC_H_
#define RTC_H_

//definitions for RTC commands
#define READ_STATUS_REG1                0x61
#define WRITE_STATUS_REG1               0x60
#define READ_STATUS_REG2                0x63
#define WRITE_STATUS_REG2               0x62
#define READ_REALTIME_DATA1             0x65
#define WRITE_REALTIME_DATA1            0x64
#define READ_REALTIME_DATA2             0x67
#define WRITE_REALTIME_DATA2            0x66
#define READ_INT1_REG                   0x69
#define WRITE_INT1_REG                  0x68
#define READ_INT2_REG                   0x6b
#define WRITE_INT2_REG                  0x6a
#define READ_CLK_CORRECTION_REG         0x6d
#define WRITE_CLK_CORRECTION_REG        0x6c
#define READ_FREE_REG                   0x6f
#define WRITE_FREE_REG                  0x6e


//function prototypes
int RTCInit(void);
int RTCSetTime(void);
int RTCTimeGet(void);
void RTCTimeConvert(void);
unsigned char reverse(unsigned char b);
void ArrayFlip(unsigned char array[], unsigned int arrayLength);

#endif /* RTC_H_ */
