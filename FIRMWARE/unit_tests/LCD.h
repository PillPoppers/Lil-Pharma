#ifndef LCD_H
#define LCD_H

#include "fonts.h"

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

// Color definitions
#define HX8357_BLACK   0x0000
#define HX8357_BLUE    0x001F
#define HX8357_RED     0xF800
#define HX8357_GREEN   0x07E0
#define HX8357_CYAN    0x07FF
#define HX8357_MAGENTA 0xF81F
#define HX8357_YELLOW  0xFFE0
#define HX8357_WHITE   0xFFFF


unsigned char g_ucTxBuff; // tx buffer
unsigned char g_ucRxBuff; // rx buffer


//TODO: May want to convert this to a typedef struct if class continues to cause problems.
class Point{
public: 
	int x; 
	int y;  
};

void SetDC();

void ResetDC();

void writeCommand(unsigned char command);

void writeParameter(unsigned char parameter);

void SPISendByte(unsigned char data);

void InitializeLCD();

void SetBounds(unsigned short rs, unsigned short re,unsigned short cs, unsigned short ce);

void paint(unsigned long color);

void fillRect(unsigned short x, unsigned short y, unsigned short l, unsigned short h, unsigned long color);		// Method used to fill a rectangle with a certain color (this will be used for most draws), 

void drawPixel(unsigned short x, unsigned short y, unsigned long color);										// Paint in 1 pixel

void drawHLine(unsigned short x, unsigned short y, unsigned short l, unsigned long color);						// Draw a horizontal line

void drawVLine(unsigned short x, unsigned short y, unsigned short h, unsigned long color);						// Draw a vertical line

void fillScreen(unsigned long color);																			// Fills entire screen up

void drawBitmap(unsigned short x, unsigned short y, unsigned short l, unsigned short h,
                unsigned short scale, unsigned char bitmap[], unsigned long foreColor, unsigned long backColor); // A .bmp image can be loaded and this method will paint it onto the screen

void writeChar(unsigned short x, unsigned short y, unsigned short scale, char text,
               unsigned long foreColor, unsigned long backColor);                                               // Write character indicated to it

void writeText(unsigned short x, unsigned short y, unsigned short scale, char * text,
               unsigned long foreColor, unsigned long backColor);

#endif
