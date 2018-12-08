#ifndef LCD_H
#define LCD_H

#include <msp430.h>
#include "spi.h"
#include "util.h"

#define SWRESET     0x01
#define ALLPOFF     0x22
#define ALLPON      0x23
#define SLPIN       0x10
#define SLPOUT      0x11
#define DISPOFF     0x28
#define DISPON      0x29
#define MADCTL      0x36
#define COLMOD      0x3A
#define SETRGB      0xB3
#define CASET       0x2A
#define PASET       0x2B
#define RAMWR       0x2C

#define LENGTH      480
#define HEIGHT      320


void Delay();
void InitializeLCD();


void SetBounds(unsigned short rs, unsigned short re,unsigned short cs, unsigned short ce);
void paint(unsigned long color);

void fillRect(unsigned short x, unsigned short y, unsigned short l, unsigned short h, unsigned long color);		// Method used to fill a rectangle with a certain color (this will be used for most draws), 
void drawPixel(unsigned short x, unsigned short y, unsigned long color);										// Paint in 1 pixel
void drawHLine(unsigned short x, unsigned short y, unsigned short l, unsigned long color);						// Draw a horizontal line
void drawVLine(unsigned short x, unsigned short y, unsigned short h, unsigned long color);						// Draw a vertical line
void writeText(unsigned short x, unsigned short y, unsigned char * text, unsigned short scale, unsigned long color);				  	// Write text indicated to it
void fillScreen(unsigned long color);																			// Fills entire screen up
void drawBitmap(unsigned short x, unsigned short y, unsigned short l, unsigned short h, unsigned short scale, unsigned char bitmap[]); 	// A .bmp image can be loaded and this method will paint it onto the screen


#endif
