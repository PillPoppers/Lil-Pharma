#include "LCD.h"

void InitializeLCD()
{
    writeCommand(SWRESET);
    Delay();

    writeCommand(SETRGB);
    writeParameter(0x80);
    writeParameter(0x00);
    writeParameter(0x06);
    writeParameter(0x06);

    writeCommand(MADCTL);
    writeParameter(0x20);

    writeCommand(COLMOD);
    writeParameter(0x66);

    writeCommand(SLPOUT);
    Delay();

    writeCommand(DISPON);
    Delay();
}

void Delay()
{
    // Add in delay if LCD does not get sufficient time to reset
    //UtilsDelay(100000);
}

void SetBounds(unsigned short rs, unsigned short re,unsigned short cs, unsigned short ce)
{
    writeCommand(CASET);
    writeParameter(rs>>8);
    writeParameter(rs);
    writeParameter(re>>8);
    writeParameter(re);

    writeCommand(PASET);
    writeParameter(cs>>8);
    writeParameter(cs);
    writeParameter(ce>>8);
    writeParameter(ce);
}

void paint(unsigned long color)
{
    writeParameter(color);
    writeParameter(color>>8);
    writeParameter(color>>16);
}

void fillRect(unsigned short x, unsigned short y, unsigned short l, unsigned short h, unsigned long color)
{
    SetBounds(x, x+l-1, y, y+h-1);

    writeCommand(RAMWR);

    unsigned short i;
    unsigned long n = l*h;
    for (i = 0; i < n; i++)
            paint(color);
}

void drawPixel(unsigned short x, unsigned short y, unsigned long color)
{
    fillRect(x, y, 1, 1, color);
}

void drawHLine(unsigned short x, unsigned short y, unsigned short l, unsigned long color)
{
    fillRect(x, y, l, 1, color);
}

void drawVLine(unsigned short x, unsigned short y, unsigned short h, unsigned long color)
{
    fillRect(x, y, 1, h, color);
}

void fillScreen(unsigned long color)
{
    fillRect(0, 0, LENGTH-1, HEIGHT-1, color);
}

void drawBitmap(unsigned short x, unsigned short y, unsigned short l, unsigned short h,
                unsigned short scale, unsigned char bitmap[], unsigned long foreColor, unsigned long backColor)
{
    SetBounds(x, x+(l*scale)-1, y, y+(h*scale)-1);

    writeCommand(RAMWR);

    unsigned int i, j, p, q, n;
    n = 0;
    for (i = 0; i < h; i++)
    {
        for (p = 0; p < scale; p++)
        {
            for (j = 0; j < l; j++)
            {
                for (q = 0; q < scale; q++)
                {
                    n = (i*h)+j;
                    if (bitmap[n] > 220)
                        paint(foreColor);
                    else
                        paint(backColor);
                }
            }
        }
    }
}

void writeChar(unsigned short x, unsigned short y, unsigned short scale, char text,
               unsigned long foreColor, unsigned long backColor)
{
    unsigned short line; // horizontal row of pixels of character
    int col, row, i, j; // loop indices

    SetBounds(x, x + 6 * scale - 1, y,  y + 8 * scale - 1);
    writeCommand(RAMWR);

    line = 0x01;        // print the top row first
    // print the rows, starting at the top
    for (row = 0; row < 8; row++)
    {
        for (i = 0; i < scale; i++)
        {
            // print the columns, starting on the left
            for (col = 0; col < 5; col++)
            {
                if (Font[(text * 5) + col] & line)
                {
                    // bit is set in Font, print pixel(s) in text color
                    for (j = 0; j < scale; j++)
                    {
                        paint(foreColor);
                    }
                }
                else
                {
                    // bit is cleared in Font, print pixel(s) in background color
                    for (j = 0; j < scale; j++)
                    {
                        paint(backColor);
                    }
                }
            }
            // print blank column(s) to the right of character
            for (j = 0; j < scale; j++)
            {
                paint(backColor);
            }
        }
        line = line << 1;   // move up to the next row
    }
}

void writeText(unsigned short x, unsigned short y, unsigned short scale, char * text,
               unsigned long foreColor, unsigned long backColor)
{
    unsigned short x_total = scale * 6;
    while (*text){
        writeChar(x, y, scale, *text, foreColor, backColor);
        text++;
        x +=x_total;
    }
}


