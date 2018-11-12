#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "spi.h"
#include "prcm.h"
#include "pinmux.h"


#define SPI_IF_BIT_RATE  100000

void InitializeLCDCommunication();

void writeCommand(unsigned char command);
void writeParameter(unsigned char parameter);

void SPISendByte(unsigned char data);

#endif
