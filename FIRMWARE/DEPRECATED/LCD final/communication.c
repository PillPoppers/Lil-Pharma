#include "communication.h"

unsigned char g_ucTxBuff;
unsigned char g_ucRxBuff;

void InitializeLCDCommunication()
{
    //
    // Enable the SPI module clock
    //
    PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);

    //
    // Reset the peripheral
    //
    PRCMPeripheralReset(PRCM_GSPI);

    //
    // Reset SPI
    //
    SPIReset(GSPI_BASE);

    //
    // Configure SPI interface
    //
    SPIConfigSetExpClk(GSPI_BASE,PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_HW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVELOW |
                     SPI_WL_8));

    //
    // Enable SPI for communication
    //
    SPIEnable(GSPI_BASE);
}

void writeCommand(unsigned char command)
{
    ResetDC();
    SPISendByte(command);
}

void writeParameter(unsigned char parameter)
{
    SetDC();
    SPISendByte(parameter);
}

void SPISendByte(unsigned char data)
{
    // Pushing them to end
    g_ucTxBuff = data;


    SPITransfer(GSPI_BASE, &g_ucTxBuff, &g_ucRxBuff, 1,
                        SPI_CS_ENABLE|SPI_CS_DISABLE);


}

