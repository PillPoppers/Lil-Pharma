#ifndef SPI_H
#define SPI_H

#include <msp430.h>
#include "util.h"

//  MOSI

#define MOSI_BIT                 BIT7
#define MOSI_PORT                P1OUT
#define MOSI_DDR                 P1DIR
#define SET_MOSI                 MOSI_PORT |= MOSI_BIT
#define RESET_MOSI               MOSI_PORT &= ~MOSI_BIT
#define SET_MOSI_AS_AN_OUTPUT    MOSI_DDR |= MOSI_BIT

//  SCK

#define SCLK_BIT                 BIT5
#define SCLK_PORT                P1OUT // SPI Slave Clock output
#define SCLK_DDR                 P1DIR // SPI Slave Clock direction
#define SET_SCLK_AS_AN_OUTPUT    SCLK_DDR |= SCLK_BIT
#define SET_SCLK                 SCLK_PORT |= SCLK_BIT
#define TOGGLE_SCLK              SCLK_PORT ^= SCLK_BIT


// CS

#define CS_BIT                 BIT0
#define CS_PORT                P2OUT
#define CS_DDR                 P2DIR
#define SET_CS_AS_AN_OUTPUT    CS_DDR |= CS_BIT
#define SET_CS                 CS_PORT |= CS_BIT
#define RESET_CS               CS_PORT &= ~CS_BIT

void InitializeSPI();

void writeCommand(unsigned char command);
void writeParameter(unsigned char parameter);

void writeCommandB(unsigned char commands[]);
void writeParameterB(unsigned char parameters[]);


void SPISendByte(unsigned char DCx, unsigned char data[]);

#endif
