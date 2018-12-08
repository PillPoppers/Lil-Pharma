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

    writeCommand(COLMOD);
    writeParameter(0x66);

    writeCommand(SLPOUT);
    Delay();

    writeCommand(DISPON);
    Delay();
}

void Delay()
{
    _delay_cycles(100000);
}

void SetBounds(unsigned short rs, unsigned short re,unsigned short cs, unsigned short ce)
{
    writeCommand(PASET);
    writeParameter(rs>>8);
    writeParameter(rs);
    writeParameter(re>>8);
    writeParameter(re);

    writeCommand(CASET);
    writeParameter(cs>>8);
    writeParameter(cs);
    writeParameter(ce>>8);
    writeParameter(ce);
}

void paint(unsigned long color)
{
    writeParameter(color>>16);
    writeParameter(color>>8);
    writeParameter(color);
}

void fillRect(unsigned short x, unsigned short y, unsigned short l, unsigned short h, unsigned long color)
{
    SetBounds(x, x+l-1, y, y+h-1);

    writeCommand(RAMWR);

    unsigned short i, j;
    unsigned long n = l*h;
    for (i = 0; i < l; i++)
        for (j = 0; j < h; j++)
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

void drawBitmap(unsigned short x, unsigned short y, unsigned short l, unsigned short h, unsigned short scale, unsigned char bitmap[])
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
                        paint(WHITE);
                    else
                        paint(BLACK);
                }
            }
        }
    }
}

