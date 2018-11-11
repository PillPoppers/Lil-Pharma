#ifndef CONVERTER_H
#define CONVERTER_H

#define BLACK   0x0
#define WHITE   0xFFFFFF
#define RED     0xFF0000
#define LIME    0x00FF00
#define BLUE    0x0000FF
#define YELLOW  0xFFFF00
#define CYAN    0x00FFFF
#define MAGENTA 0xFF00FF
#define SILVER  0xC0C0C0
#define GRAY    0x808080
#define MAROON  0x800000
#define OLIVE   0x808000
#define GREEN   0x8000
#define PURPLE  0x800080
#define TEAL    0x8080
#define NAVY    0x80

// For more color combos: https://www.rapidtables.com/web/color/RGB_Color.html

unsigned char BinToChar(unsigned char * binary);

unsigned char * CharToBin(unsigned char character);

#endif
