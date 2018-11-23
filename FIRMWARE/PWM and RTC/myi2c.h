/*
 * rtc.h
 *
 *  Created on: Oct 28, 2018
 *      Author: benja
 */

#ifndef MYI2C_H_
#define MYI2C_H_


// definitions for I2C interface
#define I2C_BASE            I2CA0_BASE
#define I2C_TIMEOUT_VAL     0x7d
#define I2C_MRIS_CLKTOUT    0x2
#define RTC_ADDRESS         0x30
#define CLOCK_SPEED         80000000


void MyI2CInit(void);
void MyI2CDisable(void);
void MyI2CCommand(unsigned long command);
void MyI2CWrite(unsigned char *DataPt, unsigned char DataLength, unsigned char StopBit);
void MyI2CRead(unsigned char *DataPt, unsigned char DataLength);


#endif /* MYI2C_H_ */
