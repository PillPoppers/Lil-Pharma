#include "spi.h"

void InitializeSPI()
{
    SET_MOSI_AS_AN_OUTPUT;

    SET_SCLK_AS_AN_OUTPUT;
    SET_SCLK;

    SET_CS_AS_AN_OUTPUT;
    SET_CS;
}

void writeCommand(unsigned char command)
{
    writeCommandB(CharToBin(command));
}

void writeParameter(unsigned char parameter)
{
    writeParameterB(CharToBin(parameter));
}

void writeCommandB(unsigned char commands[])
{
    SPISendByte(0, commands);
}

void writeParameterB(unsigned char parameters[])
{
    SPISendByte(1, parameters);
}

void SPISendByte(unsigned char DCx, unsigned char data[])
{
    unsigned short k;

    RESET_CS;
    //_delay_cycles(10000);


    if (DCx)
        SET_MOSI;
    else
        RESET_MOSI;
    //_delay_cycles(6000);



    TOGGLE_SCLK;
    //_delay_cycles(10000);

    TOGGLE_SCLK;
    //_delay_cycles(10000);

    for (k = 0; k < 8; k++){
        if (data[k])
            SET_MOSI;
        else
            RESET_MOSI;
        //_delay_cycles(6000);

        TOGGLE_SCLK;
        //_delay_cycles(10000);

        TOGGLE_SCLK;
        //_delay_cycles(10000);
    }

    SET_CS;
    //_delay_cycles(10000);
}



