// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include <spi.h>
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "uart.h"
#include "interrupt.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"
#include "timer.h"
#include "Adafruit_HX8357.h"

#include "arduino.h"


#define MADCTL_MY  0x80     ///< Bottom to top
#define MADCTL_MX  0x40     ///< Right to left
#define MADCTL_MV  0x20     ///< Reverse Mode
#define MADCTL_ML  0x10     ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00     ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08     ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04     ///< LCD refresh right to left


#define LCD_RESET_HIGH() GPIOPinWrite(GPIOA0_BASE, 0x01, 0x01); // Toggle LCD_RESET HIGH. (GPIO00, PIN_50 on Launchpad, pin_44 on MOD)
#define LCD_RESET_LOW()  GPIOPinWrite(GPIOA0_BASE, 0x01, 0x00);
#define SPI_DC_HIGH()    GPIOPinWrite(GPIOA1_BASE, 0x10, 0x10); // Toggle LCD_DC HIGH (pin also called RS. GPIO12, PIN_03 on Launchpad, pin_09 on MOD)
#define SPI_DC_LOW()     GPIOPinWrite(GPIOA1_BASE, 0x10, 0x00);
#define SPI_CS_HIGH()    GPIOPinWrite(GPIOA2_BASE, 0x02, 0x40);  // set LCD_CS HIGH (done with actual CS pin used as GPIO)
#define SPI_CS_LOW()     GPIOPinWrite(GPIOA2_BASE, 0x02, 0x00);

#define HSPI_SET_CLOCK() //SPI_OBJECT.setClockDivider(SPI_CLOCK_DIV2);

#define SPI_BEGIN()             SPIEnable( GSPI_BASE )
#define SPI_BEGIN_TRANSACTION() //SPIDisable(GSPI_BASE)          //< SPI begin transaction
#define SPI_END_TRANSACTION()   //HSPI_END_TRANSACTION();        //< SPI end transaction

// hardware SPI read
unsigned long HSPI_READ(){
    unsigned long data;
    SPIDataGet(GSPI_BASE,&data);
    return data;
}

// hardware SPI write 8 bits
#define SPI_WRITE(b)\
    SPICSEnable(GSPI_BASE);\
    SPIDataPut(GSPI_BASE,(b));\
    SPICSDisable(GSPI_BASE);

// hardware SPI write 16 bits
#define SPI_WRITE16(s)\
    SPICSEnable(GSPI_BASE);\
    SPIDataPut(GSPI_BASE,(s) >> 8);\
    SPIDataPut(GSPI_BASE,(s));\
    SPICSDisable(GSPI_BASE);

// Hardware SPI write 32 bits
#define SPI_WRITE32(l)\
    SPICSEnable(GSPI_BASE);\
    SPIDataPut(GSPI_BASE,(l) >> 24);\
    SPIDataPut(GSPI_BASE,(l) >> 16);\
    SPIDataPut(GSPI_BASE,(l) >> 8);\
    SPIDataPut(GSPI_BASE,(l));\
    SPICSDisable(GSPI_BASE);

#define SPI_WRITE_PIXELS(c,l)\
    for(uint32_t i=0; i<((l)/2); i++){\
        SPI_WRITE16(((uint16_t*)(c))[i]);\
    }
// #define SPI_WRITE_PIXELS(c,l)
//  for(uint32_t i=0; i<((l)/2); i++){\
//     SPI_WRITE16(((uint16_t*)(c))[i]);
//  }

// Hardware SPI write 'l' pixels 16-bits each
//#define SPI_WRITE_PIXELS(c,l)\
//  for(uint32_t i=0; i<(l); i+=2){\
//    HSPI_WRITE(((uint8_t*)(c))[i+1]);\
//    HSPI_WRITE(((uint8_t*)(c))[i]);\
//  }\
// Standard Byte-by-Byte SPI
//static inline uint8_t _avr_spi_read(void) {
//    uint8_t r = 0;
//    SPDR = r;
//    while(!(SPSR & _BV(SPIF)));
//    r = SPDR;
//    return r;
//}



/**************************************************************************/
/*!
    @brief  Pass 8-bit (each) R,G,B, get back 16-bit packed color
            This function converts 8-8-8 RGB data to 16-bit 5-6-5
    @param    red   Red 8 bit color
    @param    green Green 8 bit color
    @param    blue  Blue 8 bit color
    @return   Unsigned 16-bit down-sampled color in 5-6-5 format
*/
/**************************************************************************/
uint16_t Adafruit_HX8357::color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit HX8357 driver. Uses hardware SPI
*/
/**************************************************************************/
Adafruit_HX8357::Adafruit_HX8357() : Adafruit_GFX(HX8357_TFTWIDTH, HX8357_TFTHEIGHT) {

}

/**************************************************************************/
/*!
    @brief   Initialize HX8357 chip
    Connects to the HX8357 over SPI and sends initialization procedure commands
    @param    type Sub-type of HX8357, Currently only HX8357D is supported
*/
/**************************************************************************/
void Adafruit_HX8357::begin(uint8_t type) {
  
  // toggle RST low to reset and get LCD into known state
  
  while(1){
      SPICSEnable(GSPI_BASE);
      for(char _i = 0; _i < 5; _i++){
          SPIDataPut(GSPI_BASE,(_i));
          //SPIDataGet();
      }
      SPICSDisable(GSPI_BASE);
  }


  LCD_RESET_HIGH()
  delay(100);
  LCD_RESET_LOW()
  delay(100);
  LCD_RESET_HIGH()
  delay(200);

  startWrite();
  writeCommand(HX8357_SWRESET);
  delay(10);

  // setextc
  writeCommand(HX8357D_SETC);
  spiWrite(0xFF);
  spiWrite(0x83);
  spiWrite(0x57);
  delay(300);

  // setRGB which also enables SDO
  writeCommand(HX8357_SETRGB); 
  spiWrite(0x80);  //enable SDO pin!
  //spiWrite(0x00);  //disable SDO pin!
  spiWrite(0x0);
  spiWrite(0x06);
  spiWrite(0x06);

  writeCommand(HX8357D_SETCOM);
  spiWrite(0x25);  // -1.52V
    
  writeCommand(HX8357_SETOSC);
  spiWrite(0x68);  // Normal mode 70Hz, Idle mode 55 Hz
    
  writeCommand(HX8357_SETPANEL); //Set Panel
  spiWrite(0x05);  // BGR, Gate direction swapped
  
  writeCommand(HX8357_SETPWR1);
  spiWrite(0x00);  // Not deep standby
  spiWrite(0x15);  //BT
  spiWrite(0x1C);  //VSPR
  spiWrite(0x1C);  //VSNR
  spiWrite(0x83);  //AP
  spiWrite(0xAA);  //FS
  writeCommand(HX8357D_SETSTBA);  
  spiWrite(0x50);  //OPON normal
  spiWrite(0x50);  //OPON idle
  spiWrite(0x01);  //STBA
  spiWrite(0x3C);  //STBA
  spiWrite(0x1E);  //STBA
  spiWrite(0x08);  //GEN
  
  writeCommand(HX8357D_SETCYC);  
  spiWrite(0x02);  //NW 0x02
  spiWrite(0x40);  //RTN
  spiWrite(0x00);  //DIV
  spiWrite(0x2A);  //DUM
  spiWrite(0x2A);  //DUM
  spiWrite(0x0D);  //GDON
  spiWrite(0x78);  //GDOFF
  writeCommand(HX8357D_SETGAMMA); 
  spiWrite(0x02);
  spiWrite(0x0A);
  spiWrite(0x11);
  spiWrite(0x1d);
  spiWrite(0x23);
  spiWrite(0x35);
  spiWrite(0x41);
  spiWrite(0x4b);
  spiWrite(0x4b);
  spiWrite(0x42);
  spiWrite(0x3A);
  spiWrite(0x27);
  spiWrite(0x1B);
  spiWrite(0x08);
  spiWrite(0x09);
  spiWrite(0x03);
  spiWrite(0x02);
  spiWrite(0x0A);
  spiWrite(0x11);
  spiWrite(0x1d);
  spiWrite(0x23);
  spiWrite(0x35);
  spiWrite(0x41);
  spiWrite(0x4b);
  spiWrite(0x4b);
  spiWrite(0x42);
  spiWrite(0x3A);
  spiWrite(0x27);
  spiWrite(0x1B);
  spiWrite(0x08);
  spiWrite(0x09);
  spiWrite(0x03);
  spiWrite(0x00);
  spiWrite(0x01);
  
  writeCommand(HX8357_COLMOD);
  spiWrite(0x55);  // 16 bit
  
  writeCommand(HX8357_MADCTL);  
  spiWrite(0xC0); 
  
  writeCommand(HX8357_TEON);  // TE off
  spiWrite(0x00); 
  
  writeCommand(HX8357_TEARLINE);  // tear line
  spiWrite(0x00); 
  spiWrite(0x02);

  writeCommand(HX8357_SLPOUT); //Exit Sleep
  delay(150);
  
  writeCommand(HX8357_DISPON);  // display on
  delay(50);
  
  endWrite();
  _width  = HX8357_TFTWIDTH;
  _height = HX8357_TFTHEIGHT;
}


/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Adafruit_HX8357::setRotation(uint8_t m) {

  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     m = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
     _width  = HX8357_TFTWIDTH;
     _height = HX8357_TFTHEIGHT;
     break;
   case 1:
     m = MADCTL_MV | MADCTL_MY | MADCTL_RGB;
     _width  = HX8357_TFTHEIGHT;
     _height = HX8357_TFTWIDTH;
     break;
  case 2:
    m = MADCTL_RGB;
     _width  = HX8357_TFTWIDTH;
     _height = HX8357_TFTHEIGHT;
    break;
   case 3:
     m = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
     _width  = HX8357_TFTHEIGHT;
     _height = HX8357_TFTWIDTH;
     break;
  }
  startWrite();
  writeCommand(HX8357_MADCTL);
  spiWrite(m);
  endWrite();
}


/**************************************************************************/
/*!
    @brief   Enable/Disable display color inversion
    @param   invert True to invert, False to have normal color
*/
/**************************************************************************/
void Adafruit_HX8357::invertDisplay(bool invert) {
  startWrite();
  writeCommand(invert ? HX8357_INVON : HX8357_INVOFF);
  endWrite();
}


/**************************************************************************/
/*!
    @brief   Set the "address window" - the rectangle we will write to RAM with the next chunk of SPI data writes. The HX8357 will automatically wrap the data as each row is filled
    @param   x  TFT memory 'x' origin
    @param   y  TFT memory 'y' origin
    @param   w  Width of rectangle
    @param   h  Height of rectangle
*/
/**************************************************************************/
void Adafruit_HX8357::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
  uint32_t ya = ((uint32_t)y << 16) | (y+h-1);
  writeCommand(HX8357_CASET); // Column addr set
  SPI_WRITE32(xa);

  writeCommand(HX8357_PASET); // Row addr set
  SPI_WRITE32(ya);

  writeCommand(HX8357_RAMWR); // write to RAM
}


/**************************************************************************/
/*!
    @brief   Blit 1 pixel of color, sets up SPI transaction
    @param   color 16-bits of 5-6-5 color data
*/
/**************************************************************************/
void Adafruit_HX8357::pushColor(uint16_t color) {
  SPI_WRITE16(color);
}

/**************************************************************************/
/*!
    @brief   Blit 1 pixel of color without setting up SPI transaction
    @param   color 16-bits of 5-6-5 color data
*/
/**************************************************************************/
void Adafruit_HX8357::writePixel(uint16_t color){
    SPI_WRITE16(color);
}


/**************************************************************************/
/*!
    @brief   Blit 'len' pixels of color without setting up SPI transaction
    @param   colors Array of 16-bit 5-6-5 color data
    @param   len Number of 16-bit pixels in colors array
*/
/**************************************************************************/
void Adafruit_HX8357::writePixels(uint16_t * colors, uint32_t len){
    SPI_WRITE_PIXELS((uint8_t*)colors , len * 2);
}

/**************************************************************************/
/*!
    @brief   Blit 'len' pixels of a single color without setting up SPI transaction
    @param   color 16-bits of 5-6-5 color data
    @param   len Number of 16-bit pixels you want to write out with same color
*/
/**************************************************************************/
void Adafruit_HX8357::writeColor(uint16_t color, uint32_t len){
#ifdef SPI_HAS_WRITE_PIXELS
    if(_sclk >= 0){
        for (uint32_t t=0; t<len; t++){
            writePixel(color);
        }
        return;
    }
    static uint16_t temp[SPI_MAX_PIXELS_AT_ONCE];
    size_t blen = (len > SPI_MAX_PIXELS_AT_ONCE)?SPI_MAX_PIXELS_AT_ONCE:len;
    uint16_t tlen = 0;

    for (uint32_t t=0; t<blen; t++){
        temp[t] = color;
    }

    while(len){
        tlen = (len>blen)?blen:len;
        writePixels(temp, tlen);
        len -= tlen;
    }
#else
    uint8_t hi = color >> 8, lo = color;
    if(_sclk < 0){ //AVR Optimization
        for (uint32_t t=len; t; t--){
            SPI_WRITE(hi);
            SPI_WRITE(lo);
        }
        return;
    }
    for (uint32_t t=len; t; t--){
        spiWrite(hi);
        spiWrite(lo);
    }
#endif
}

/**************************************************************************/
/*!
   @brief  Draw a single pixel, DOES NOT set up SPI transaction
    @param    x  TFT X location
    @param    y  TFT Y location
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_HX8357::writePixel(int16_t x, int16_t y, uint16_t color) {
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x,y,1,1);
    writePixel(color);
}

/**************************************************************************/
/*!
   @brief  Fill a rectangle, DOES NOT set up SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    w  Width of rectangle
    @param    h  Height of rectangle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_HX8357::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    if((x >= _width) || (y >= _height)) return;
    int16_t x2 = x + w - 1, y2 = y + h - 1;
    if((x2 < 0) || (y2 < 0)) return;

    // Clip left/top
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if(x2 >= _width)  w = _width  - x;
    if(y2 >= _height) h = _height - y;

    int32_t len = (int32_t)w * h;
    setAddrWindow(x, y, w, h);
    writeColor(color, len);
}

/**************************************************************************/
/*!
   @brief  Draw a vertical line, DOES NOT set up SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_HX8357::writeFastVLine(int16_t x, int16_t y, int16_t l, uint16_t color){
    writeFillRect(x, y, 1, l, color);
}

/**************************************************************************/
/*!
   @brief  Draw a horizontal line, DOES NOT set up SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_HX8357::writeFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color){
    writeFillRect(x, y, l, 1, color);
}


/**************************************************************************/
/*!
   @brief  Draw a single pixel, includes code for SPI transaction
    @param    x  TFT X location
    @param    y  TFT Y location
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_HX8357::drawPixel(int16_t x, int16_t y, uint16_t color) {
    startWrite();
    writePixel(x, y, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Draw a vertical line, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_HX8357::drawFastVLine(int16_t x, int16_t y, int16_t l, uint16_t color) {
    startWrite();
    writeFastVLine(x, y, l, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Draw a horizontal line, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_HX8357::drawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color) {
    startWrite();
    writeFastHLine(x, y, l, color);
    endWrite();
}


/**************************************************************************/
/*!
   @brief  Fill a rectangle, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    w  Width of rectangle
    @param    h  Height of rectangle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_HX8357::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    startWrite();
    writeFillRect(x,y,w,h,color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Fill the screen with a color
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_HX8357::fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}



/**************************************************************************/
/*!
   @brief  Draw RGB rectangle of data from RAM to a location on screen
   Adapted from https://github.com/PaulStoffregen/ILI9341_t3
   by Marc MERLIN. See examples/pictureEmbed to use this.
   5/6/2017: function name and arguments have changed for compatibility
   with current GFX library and to avoid naming problems in prior
   implementation.  Formerly drawBitmap() with arguments in different order.
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    pcolors Pointer to 16-bit color data
    @param    w  Width of pcolors rectangle
    @param    h  Height of pcolors rectangle
*/
/**************************************************************************/
void Adafruit_HX8357::drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h) {

    int16_t x2, y2; // Lower-right coord
    if(( x             >= _width ) ||      // Off-edge right
       ( y             >= _height) ||      // " top
       ((x2 = (x+w-1)) <  0      ) ||      // " left
       ((y2 = (y+h-1)) <  0)     ) return; // " bottom

    int16_t bx1=0, by1=0, // Clipped top-left within bitmap
            saveW=w;      // Save original bitmap width value
    if(x < 0) { // Clip left
        w  +=  x;
        bx1 = -x;
        x   =  0;
    }
    if(y < 0) { // Clip top
        h  +=  y;
        by1 = -y;
        y   =  0;
    }
    if(x2 >= _width ) w = _width  - x; // Clip right
    if(y2 >= _height) h = _height - y; // Clip bottom

    pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    startWrite();
    setAddrWindow(x, y, w, h); // Clipped area
    while(h--) { // For each (clipped) scanline...
      writePixels(pcolors, w); // Push one (clipped) row
      pcolors += saveW; // Advance pointer by one full (unclipped) line
    }
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Read 8-bit data via hardware or software SPI. Does not set up SPI transaction.
   @returns One byte of data from SPI
*/
/**************************************************************************/
uint8_t Adafruit_HX8357::spiRead() {
    if(_sclk < 0){
        return HSPI_READ();
    }
    if(_miso < 0){
        return 0;
    }
}

/**************************************************************************/
/*!
   @brief  Write 8-bit data via hardware or software SPI. Does not set up SPI transaction.
   @param  b Byte of data to write over SPI
*/
/**************************************************************************/
void Adafruit_HX8357::spiWrite(uint8_t b) {
  SPI_WRITE(b);
  return;
}

/**************************************************************************/
/*!
   @brief  Begin SPI transaction, for software or hardware SPI
*/
/**************************************************************************/
void Adafruit_HX8357::startWrite(void){
  //SPI_BEGIN_TRANSACTION();
  SPI_CS_LOW();
}

/**************************************************************************/
/*!
   @brief  End SPI transaction, for software or hardware SPI
*/
/**************************************************************************/
void Adafruit_HX8357::endWrite(void){
  SPI_CS_HIGH();
  //SPI_END_TRANSACTION();
}


/**************************************************************************/
/*!
   @brief  Write 8-bit data to command/register (DataCommand line low).
   Does not set up SPI transaction.
   @param  cmd The command/register to transmit
*/
/**************************************************************************/
void Adafruit_HX8357::writeCommand(uint8_t c) {
    SPI_DC_LOW();
    spiWrite(c);
    SPI_DC_HIGH();
}

////////// stuff not actively being used, but kept for posterity

/**************************************************************************/
/*!
   @brief  Read 8 bits of data from HX8357 configuration memory. NOT from RAM!
           This is highly undocumented/supported.
    @param    command  The command register to read data from
    @param    index  The byte index into the command to read from
    @return   Unsigned 8-bit data read from HX8357 register
*/
/**************************************************************************/
uint8_t Adafruit_HX8357::readcommand8(uint8_t command, uint8_t index) {
   SPI_DC_LOW();
   SPI_CS_LOW();

   spiWrite(command);
 
   SPI_DC_HIGH();
   uint8_t r = spiRead();
   SPI_CS_HIGH();
   return r;
}


/*
unsigned long testFillScreen() {
//    unsigned long start = micros();
    tft.fillScreen(HX8357_RED);
    tft.fillScreen(HX8357_GREEN);
    tft.fillScreen(HX8357_BLUE);
    tft.fillScreen(HX8357_WHITE);
    return 0;//micros() - start;
}

unsigned long testText() {
    tft.fillScreen(HX8357_BLACK);
//    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(1);
//    tft.println("Hello World!");
    tft.setTextColor(HX8357_YELLOW); tft.setTextSize(2);
//    tft.println(1234.56);
    tft.setTextColor(HX8357_RED);    tft.setTextSize(3);
//    tft.println(0xDEADBEEF, HEX);
//    tft.println();
    tft.setTextColor(HX8357_GREEN);
    tft.setTextSize(5);
//    tft.println("Groop");
    tft.setTextSize(2);
//    tft.println("I implore thee,");
    tft.setTextSize(1);
//    tft.println("my foonting turlingdromes.");
//    tft.println("And hooptiously drangle me");
//    tft.println("with crinkly bindlewurdles,");
//    tft.println("Or I will rend thee");
//    tft.println("in the gobberwarts");
//    tft.println("with my blurglecruncheon,");
//    tft.println("see if I don't!");
    tft.setTextColor(HX8357_WHITE);
//    tft.println(F("Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister was reading, but it had no pictures or conversations in it, 'and what is the use of a book,' thought Alice 'without pictures or conversations?'"));
//    tft.println(F("So she was considering in her own mind (as well as she could, for the hot day made her feel very sleepy and stupid), whether the pleasure of making a daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly a White Rabbit with pink eyes ran close by her."));
//    tft.println(F("There was nothing so very remarkable in that; nor did Alice think it so very much out of the way to hear the Rabbit say to itself, 'Oh dear! Oh dear! I shall be late!' (when she thought it over afterwards, it occurred to her that she ought to have wondered at this, but at the time it all seemed quite natural); but when the Rabbit actually took a watch out of its waistcoat-pocket, and looked at it, and then hurried on, Alice started to her feet, for it flashed across her mind that she had never before seen a rabbit with either a waistcoat-pocket, or a watch to take out of it, and burning with curiosity, she ran across the field after it, and fortunately was just in time to see it pop down a large rabbit-hole under the hedge."));
//    tft.println(F("In another moment down went Alice after it, never once considering how in the world she was to get out again."));
//    tft.println(F("The rabbit-hole went straight on like a tunnel for some way, and then dipped suddenly down, so suddenly that Alice had not a moment to think about stopping herself before she found herself falling down a very deep well."));
//    tft.println(F("Either the well was very deep, or she fell very slowly, for she had plenty of time as she went down to look about her and to wonder what was going to happen next. First, she tried to look down and make out what she was coming to, but it was too dark to see anything; then she looked at the sides of the well, and noticed that they were filled with cupboards and book-shelves; here and there she saw maps and pictures hung upon pegs. She took down a jar from one of the shelves as she passed; it was labelled 'ORANGE MARMALADE', but to her great disappointment it was empty: she did not like to drop the jar for fear of killing somebody, so managed to put it into one of the cupboards as she fell past it."));
  return 0;//micros() - start;
}

unsigned long testLines(uint16_t color) {
//    unsigned long start, t;
    int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

    tft.fillScreen(HX8357_BLACK);

    x1 = y1 = 0;
    y2    = h - 1;
//    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
//    t     = micros() - start; // fillScreen doesn't count against timing


    return 0;//micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
//  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(HX8357_BLACK);
//  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return 0;// micros() - start;
}

unsigned long testRects(uint16_t color) {
//  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(HX8357_BLACK);
  n     = min(tft.width(), tft.height());
//  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return 0;//micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(HX8357_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
//    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
//    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(HX8357_BLACK);
//  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return 0;//micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
//  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return 0;//micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(HX8357_BLACK);
  n     = min(cx, cy);
//  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(200, 20, i));
  }

  return 0;//micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(HX8357_BLACK);
//  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
//    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
//    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 ,
                cy = tft.height() / 2 ;

  tft.fillScreen(HX8357_BLACK);
  w     = min(tft.width(), tft.height());
//  start = micros();
  for(i=0; i<w; i+=8) {
    i2 = i / 2 - 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 100, 100));
  }

  return 0;// micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 + 10,
                cy = tft.height() / 2 + 10;

  tft.fillScreen(HX8357_BLACK);
//  start = micros();
  for(i=min(tft.width(), tft.height()) - 20; i>25; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i-20, i-20, i/8, tft.color565(100, i/2, 100));
  }

  return 0;//micros() - start;
}
*/
