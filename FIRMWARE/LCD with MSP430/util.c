#include <util.h>

unsigned char BinToChar(unsigned char * binary)
{
    unsigned char c = 0x00;

    unsigned short i = 0;
    for (i = 0; i < 8; i++){
        if (binary[i])
            c += (1<<(7-i));
    }

    return c;
}

unsigned char * CharToBin(unsigned char character)
{
    unsigned char b[] = {0,0,0,0,0,0,0,0};

    unsigned short k = 0;
    for (k = 0; k < 8; k++){
        if (character & 0x80)
            b[k] = 1;
        else
            b[k] = 0;

        character <<= 1;
    }
    return b;
}


