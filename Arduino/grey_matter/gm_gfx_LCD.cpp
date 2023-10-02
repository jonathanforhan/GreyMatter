#ifdef __AVR__
#   include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#   include <pgmspace.h>
#endif

#ifndef pgm_read_byte
#   define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#   define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#   define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.  Try to accommodate both...
#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#   define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#   define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#if defined(__SAM3X8E__)
#   include <include/pio.h>
#   define PROGMEM
#endif
#include "pins_arduino.h"
#include "wiring_private.h"
#include "detail/pin_magic.h"
#include "gm_gfx_LCD.hpp"
#include "gm_gfx_Font.ipp"

#define TFTWIDTH   240
#define TFTHEIGHT  320

// LCD controller chip identifiers
#define ID_932X    0
#define ID_7575    1
#define ID_9341    2
#define ID_HX8357D 3
#define ID_4535    4
#define ID_9481    5
#define ID_UNKNOWN 0xFF

#include "detail/registers.h"

#define GML_STL 1
#include "gm_stl/gm_stl.hpp"

#define GM_USE_9341 1

namespace gm::gfx {

LCD::LCD(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t reset)
      : WIDTH(TFTWIDTH),
        HEIGHT(TFTHEIGHT),
        _dimensions({ WIDTH, HEIGHT }),
        _rotation(0),
        _cursor({ 0, 0 }),
        _text_size({ 1, 1 }),
        _text_fg(0xFFFF),
        _text_bg(0xFFFF),
        _wrap(true)
{
#ifndef USE_ADAFRUIT_SHIELD_PINOUT
    // Convert pin numbers to registers and bitmasks
    _reset = reset;
#ifdef __AVR__
    csPort = portOutputRegister(digitalPinToPort(cs));
    cdPort = portOutputRegister(digitalPinToPort(cd));
    wrPort = portOutputRegister(digitalPinToPort(wr));
    rdPort = portOutputRegister(digitalPinToPort(rd));
#endif
#if defined(__SAM3X8E__)
    csPort = digitalPinToPort(cs);
    cdPort = digitalPinToPort(cd);
    wrPort = digitalPinToPort(wr);
    rdPort = digitalPinToPort(rd);
#endif
    csPinSet = digitalPinToBitMask(cs);
    cdPinSet = digitalPinToBitMask(cd);
    wrPinSet = digitalPinToBitMask(wr);
    rdPinSet = digitalPinToBitMask(rd);
    csPinUnset = ~csPinSet;
    cdPinUnset = ~cdPinSet;
    wrPinUnset = ~wrPinSet;
    rdPinUnset = ~rdPinSet;
#ifdef __AVR__
    *csPort |= csPinSet; // Set all control bits to HIGH (idle)
    *cdPort |= cdPinSet; // Signals are ACTIVE LOW
    *wrPort |= wrPinSet;
    *rdPort |= rdPinSet;
#endif
#if defined(__SAM3X8E__)
    csPort->PIO_SODR |= csPinSet; // Set all control bits to HIGH (idle)
    cdPort->PIO_SODR |= cdPinSet; // Signals are ACTIVE LOW
    wrPort->PIO_SODR |= wrPinSet;
    rdPort->PIO_SODR |= rdPinSet;
#endif
    pinMode(cs, OUTPUT); // Enable outputs
    pinMode(cd, OUTPUT);
    pinMode(wr, OUTPUT);
    pinMode(rd, OUTPUT);
    if (reset)
    {
        digitalWrite(reset, HIGH);
        pinMode(reset, OUTPUT);
    }
#endif
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
    CS_IDLE; // Set all control bits to idle state
    WR_IDLE;
    RD_IDLE;
    CD_DATA;
    digitalWrite(5, HIGH); // Reset line
    pinMode(A3, OUTPUT);   // Enable outputs
    pinMode(A2, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(5, OUTPUT);
#endif

    setWriteDir(); // Set up LCD data port(s) for WRITE operations
}

// Initialization command tables for different LCD controllers
#define TFTLCD_DELAY 0xFF
static constexpr uint8_t HX8347G_regValues[] PROGMEM = {
    0x2E           , 0x89,
    0x29           , 0x8F,
    0x2B           , 0x02,
    0xE2           , 0x00,
    0xE4           , 0x01,
    0xE5           , 0x10,
    0xE6           , 0x01,
    0xE7           , 0x10,
    0xE8           , 0x70,
    0xF2           , 0x00,
    0xEA           , 0x00,
    0xEB           , 0x20,
    0xEC           , 0x3C,
    0xED           , 0xC8,
    0xE9           , 0x38,
    0xF1           , 0x01,

    // skip gamma, do later

    0x1B           , 0x1A,
    0x1A           , 0x02,
    0x24           , 0x61,
    0x25           , 0x5C,
    
    0x18           , 0x36,
    0x19           , 0x01,
    0x1F           , 0x88,
    TFTLCD_DELAY   , 5   , // delay 5 ms
    0x1F           , 0x80,
    TFTLCD_DELAY   , 5   ,
    0x1F           , 0x90,
    TFTLCD_DELAY   , 5   ,
    0x1F           , 0xD4,
    TFTLCD_DELAY   , 5   ,
    0x17           , 0x05,

    0x36           , 0x09,
    0x28           , 0x38,
    TFTLCD_DELAY   , 40  ,
    0x28           , 0x3C,

    0x02           , 0x00,
    0x03           , 0x00,
    0x04           , 0x00,
    0x05           , 0xEF,
    0x06           , 0x00,
    0x07           , 0x00,
    0x08           , 0x01,
    0x09           , 0x3F
};

static constexpr uint8_t HX8357D_regValues[] PROGMEM = {
    HX8357_SWRESET, 0,
    HX8357D_SETC, 3, 0xFF, 0x83, 0x57,
    TFTLCD_DELAY, 250,
    HX8357_SETRGB, 4, 0x00, 0x00, 0x06, 0x06,
    HX8357D_SETCOM, 1, 0x25,  // -1.52V
    HX8357_SETOSC, 1, 0x68,  // Normal mode 70Hz, Idle mode 55 Hz
    HX8357_SETPANEL, 1, 0x05,  // BGR, Gate direction swapped
    HX8357_SETPWR1, 6, 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA,
    HX8357D_SETSTBA, 6, 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08,
    // MEME GAMMA HERE
    HX8357D_SETCYC, 7, 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78,
    HX8357_COLMOD, 1, 0x55,
    HX8357_MADCTL, 1, 0xC0,
    HX8357_TEON, 1, 0x00,
    HX8357_TEARLINE, 2, 0x00, 0x02,
    HX8357_SLPOUT, 0,
    TFTLCD_DELAY, 150,
    HX8357_DISPON, 0, 
    TFTLCD_DELAY, 50,
};

static constexpr uint16_t ILI932x_regValues[] PROGMEM = {
    ILI932X_START_OSC        , 0x0001, // Start oscillator
    TFTLCD_DELAY             , 50,     // 50 millisecond delay
    ILI932X_DRIV_OUT_CTRL    , 0x0100,
    ILI932X_DRIV_WAV_CTRL    , 0x0700,
    ILI932X_ENTRY_MOD        , 0x1030,
    ILI932X_RESIZE_CTRL      , 0x0000,
    ILI932X_DISP_CTRL2       , 0x0202,
    ILI932X_DISP_CTRL3       , 0x0000,
    ILI932X_DISP_CTRL4       , 0x0000,
    ILI932X_RGB_DISP_IF_CTRL1, 0x0,
    ILI932X_FRM_MARKER_POS   , 0x0,
    ILI932X_RGB_DISP_IF_CTRL2, 0x0,
    ILI932X_POW_CTRL1        , 0x0000,
    ILI932X_POW_CTRL2        , 0x0007,
    ILI932X_POW_CTRL3        , 0x0000,
    ILI932X_POW_CTRL4        , 0x0000,
    TFTLCD_DELAY             , 200,
    ILI932X_POW_CTRL1        , 0x1690,
    ILI932X_POW_CTRL2        , 0x0227,
    TFTLCD_DELAY             , 50,
    ILI932X_POW_CTRL3        , 0x001A,
    TFTLCD_DELAY             , 50,
    ILI932X_POW_CTRL4        , 0x1800,
    ILI932X_POW_CTRL7        , 0x002A,
    TFTLCD_DELAY             , 50,
    ILI932X_GAMMA_CTRL1      , 0x0000,
    ILI932X_GAMMA_CTRL2      , 0x0000,
    ILI932X_GAMMA_CTRL3      , 0x0000,
    ILI932X_GAMMA_CTRL4      , 0x0206,
    ILI932X_GAMMA_CTRL5      , 0x0808,
    ILI932X_GAMMA_CTRL6      , 0x0007,
    ILI932X_GAMMA_CTRL7      , 0x0201,
    ILI932X_GAMMA_CTRL8      , 0x0000,
    ILI932X_GAMMA_CTRL9      , 0x0000,
    ILI932X_GAMMA_CTRL10     , 0x0000,
    ILI932X_GRAM_HOR_AD      , 0x0000,
    ILI932X_GRAM_VER_AD      , 0x0000,
    ILI932X_HOR_START_AD     , 0x0000,
    ILI932X_HOR_END_AD       , 0x00EF,
    ILI932X_VER_START_AD     , 0X0000,
    ILI932X_VER_END_AD       , 0x013F,
    ILI932X_GATE_SCAN_CTRL1  , 0xA700, // Driver Output Control (R60h)
    ILI932X_GATE_SCAN_CTRL2  , 0x0003, // Driver Output Control (R61h)
    ILI932X_GATE_SCAN_CTRL3  , 0x0000, // Driver Output Control (R62h)
    ILI932X_PANEL_IF_CTRL1   , 0X0010, // Panel Interface Control 1 (R90h)
    ILI932X_PANEL_IF_CTRL2   , 0X0000,
    ILI932X_PANEL_IF_CTRL3   , 0X0003,
    ILI932X_PANEL_IF_CTRL4   , 0X1100,
    ILI932X_PANEL_IF_CTRL5   , 0X0000,
    ILI932X_PANEL_IF_CTRL6   , 0X0000,
    ILI932X_DISP_CTRL1       , 0x0133, // Main screen turn on
};

static constexpr uint16_t ILI9481_regValues[] PROGMEM = {	
    0x11, 0,
	TFTLCD_DELAY, 50,	
    0xD0, 3, 0x07, 0x42, 0x18,
	0xD1, 3, 0x00, 0x07, 0x10,
	
	0xD2, 2, 0x01, 0x02, 
	0xC0, 5, 0x10, 0x3B, 0x00,0x02,0x11,
	0xC5, 1, 0x03, 
	0x36, 1, 0x0A, 
	0x3A, 1, 0x55, 
	0x2A, 4, 0x00, 0x00,0x01,0x3F,
	0x2B, 4, 0x00, 0x00,0x01,0xE0,
	TFTLCD_DELAY, 50,	
	0x29, 0,
	0x2C, 0,
	TFTLCD_DELAY, 50,		
};

static constexpr uint16_t LGDP4535_regValues[] PROGMEM = {	
	//LGDP4535
	0x15,0x0030,
	0x9A,0x0010,
	0x11,0x0020,
	0x10,0x3428,
	0x12,0x0002,
	0x13,0x1046,
	TFTLCD_DELAY,40,//40ms
	0x12,0x0012,
	TFTLCD_DELAY,40,//40ms
	0x10,0x3420,
	0x13,0x3046,
	TFTLCD_DELAY,70,//70ms

	/******gamma setting******/
	0x30,0x0000,
	0x31,0x0402,
	0x32,0x0307,
	0x33,0x0304,
	0x34,0x0004,
	0x35,0x0401,
	0x36,0x0707,
	0x37,0x0305,
	0x38,0x0610,
	0x39,0x0610,

	/********display mode******/
	0x01,0x0100,
	0x02,0x0300,
	0x03,0x1030,
	0x08,0x0808,
	0x0A,0x0008,
	0x60,0x2700,
	0x61,0x0001,

	0x90,0x013E,
	0x92,0x0100,
	0x93,0x0100,
	0xA0,0x3000,
	0xA3,0x0010,

	/*******display on*******/
	0x07,0x0001,
	0x07,0x0021,
	0x07,0x0023,
	0x07,0x0033,
	0x07,0x0133,
};

void LCD::begin(uint16_t id)
{
    uint8_t i = 0;

    reset();

    delay(200);

    if (id == 0x9325 || id == 0x9328)
    {

        uint16_t a, d;
        driver = ID_932X;
        CS_ACTIVE;
        while (i < sizeof(ILI932x_regValues) / sizeof(uint16_t))
        {
            a = pgm_read_word(&ILI932x_regValues[i++]);
            d = pgm_read_word(&ILI932x_regValues[i++]);
            if (a == TFTLCD_DELAY)
                delay(d);
            else
                writeRegister16(a, d);
        }
        set_rotation(_rotation);
        setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
    }
    else if (id == 0x4535)
    {
        uint16_t a, d;
        driver = ID_932X;
        CS_ACTIVE;
        while (i < sizeof(LGDP4535_regValues) / sizeof(uint16_t))
        {
            a = pgm_read_word(&LGDP4535_regValues[i++]);
            d = pgm_read_word(&LGDP4535_regValues[i++]);
            if (a == TFTLCD_DELAY)
                delay(d);
            else
                writeRegister16(a, d);
        }
        set_rotation(_rotation);
        setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
    }
    else if (id == 0x9341)
    {

        uint16_t a, d;
        driver = ID_9341;
        CS_ACTIVE;
        writeRegister8(ILI9341_SOFTRESET, 0);
        delay(50);
        writeRegister8(ILI9341_DISPLAYOFF, 0);

        writeRegister8(ILI9341_POWERCONTROL1, 0x23);
        writeRegister8(ILI9341_POWERCONTROL2, 0x10);
        writeRegister16(ILI9341_VCOMCONTROL1, 0x2B2B);
        writeRegister8(ILI9341_VCOMCONTROL2, 0xC0);
        writeRegister8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
        writeRegister8(ILI9341_PIXELFORMAT, 0x55);
        writeRegister16(ILI9341_FRAMECONTROL, 0x001B);

        writeRegister8(ILI9341_ENTRYMODE, 0x07);
        /* writeRegister32(ILI9341_DISPLAYFUNC, 0x0A822700);*/

        writeRegister8(ILI9341_SLEEPOUT, 0);
        delay(150);
        writeRegister8(ILI9341_DISPLAYON, 0);
        delay(500);
        setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
        return;
    }
    else if (id == 0x8357)
    {
        // HX8357D
        driver = ID_HX8357D;
        CS_ACTIVE;
        while (i < sizeof(HX8357D_regValues))
        {
            uint8_t r = pgm_read_byte(&HX8357D_regValues[i++]);
            uint8_t len = pgm_read_byte(&HX8357D_regValues[i++]);
            if (r == TFTLCD_DELAY)
            {
                delay(len);
            }
            else
            {
                // Serial.print("Register $"); Serial.print(r, HEX);
                // Serial.print(" datalen "); Serial.println(len);

                CS_ACTIVE;
                CD_COMMAND;
                write8(r);
                CD_DATA;
                for (uint8_t d = 0; d < len; d++)
                {
                    uint8_t x = pgm_read_byte(&HX8357D_regValues[i++]);
                    write8(x);
                }
                CS_IDLE;
            }
        }
        return;
    }
    else if (id == 0x9481)
    {
        // 9481
        driver = ID_9341;
        CS_ACTIVE;
        while (i < sizeof(ILI9481_regValues))
        {
            uint8_t r = pgm_read_byte(&ILI9481_regValues[i++]);
            uint8_t len = pgm_read_byte(&ILI9481_regValues[i++]);
            if (r == TFTLCD_DELAY)
            {
                delay(len);
            }
            else
            {
                // Serial.print("Register $"); Serial.print(r, HEX);
                // Serial.print(" datalen "); Serial.println(len);

                CS_ACTIVE;
                CD_COMMAND;
                write8(r);
                CD_DATA;
                for (uint8_t d = 0; d < len; d++)
                {
                    uint8_t x = pgm_read_byte(&ILI9481_regValues[i++]);
                    write8(x);
                }
                CS_IDLE;
            }
        }
        return;
    }
    else if (id == 0x7575)
    {

        uint8_t a, d;
        driver = ID_7575;
        CS_ACTIVE;
        while (i < sizeof(HX8347G_regValues))
        {
            a = pgm_read_byte(&HX8347G_regValues[i++]);
            d = pgm_read_byte(&HX8347G_regValues[i++]);
            if (a == TFTLCD_DELAY)
                delay(d);
            else
                writeRegister8(a, d);
        }
        set_rotation(_rotation);
        setLR(); // Lower-right corner of address window
    }
    else
    {
        driver = ID_UNKNOWN;
        return;
    }
}

void LCD::reset()
{
    CS_IDLE;
    //  CD_DATA;
    WR_IDLE;
    RD_IDLE;

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
    digitalWrite(5, LOW);
    delay(2);
    digitalWrite(5, HIGH);
#else
    if (_reset)
    {
        digitalWrite(_reset, LOW);
        delay(2);
        digitalWrite(_reset, HIGH);
    }
#endif

    // Data transfer sync
    CS_ACTIVE;
    CD_COMMAND;
    write8(0x00);
    for (uint8_t i = 0; i < 3; i++)
        WR_STROBE; // Three extra 0x00s
    CS_IDLE;
}

// Sets the LCD address window (and address counter, on 932X).
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).
void LCD::setAddrWindow(int x1, int y1, int x2, int y2)
{
    CS_ACTIVE;
#ifndef GM_USE_9341
    if (driver == ID_932X)
    {

        // Values passed are in current (possibly rotated) coordinate
        // system.  932X requires hardware-native coords regardless of
        // MADCTL, so rotate inputs as needed.  The address counter is
        // set to the top-left corner -- although fill operations can be
        // done in any direction, the current screen rotation is applied
        // because some users find it disconcerting when a fill does not
        // occur top-to-bottom.
        int x, y, t;
        switch (_rotation)
        {
        default:
            x = x1;
            y = y1;
            break;
        case 1:
            t = y1;
            y1 = x1;
            x1 = TFTWIDTH - 1 - y2;
            y2 = x2;
            x2 = TFTWIDTH - 1 - t;
            x = x2;
            y = y1;
            break;
        case 2:
            t = x1;
            x1 = TFTWIDTH - 1 - x2;
            x2 = TFTWIDTH - 1 - t;
            t = y1;
            y1 = TFTHEIGHT - 1 - y2;
            y2 = TFTHEIGHT - 1 - t;
            x = x2;
            y = y2;
            break;
        case 3:
            t = x1;
            x1 = y1;
            y1 = TFTHEIGHT - 1 - x2;
            x2 = y2;
            y2 = TFTHEIGHT - 1 - t;
            x = x1;
            y = y2;
            break;
        }
        writeRegister16(0x0050, x1); // Set address window
        writeRegister16(0x0051, x2);
        writeRegister16(0x0052, y1);
        writeRegister16(0x0053, y2);
        writeRegister16(0x0020, x); // Set address counter to top left
        writeRegister16(0x0021, y);
    }
    else if (driver == ID_7575)
    {

        writeRegisterPair(HX8347G_COLADDRSTART_HI, HX8347G_COLADDRSTART_LO, x1);
        writeRegisterPair(HX8347G_ROWADDRSTART_HI, HX8347G_ROWADDRSTART_LO, y1);
        writeRegisterPair(HX8347G_COLADDREND_HI, HX8347G_COLADDREND_LO, x2);
        writeRegisterPair(HX8347G_ROWADDREND_HI, HX8347G_ROWADDREND_LO, y2);
    }
    else if ((driver == ID_9341) || (driver == ID_HX8357D))
    {
#endif // GM_USE_9341
        uint32_t t;

        t = x1;
        t <<= 16;
        t |= x2;
        writeRegister32(ILI9341_COLADDRSET, t); // HX8357D uses same registers!
        t = y1;
        t <<= 16;
        t |= y2;
        writeRegister32(ILI9341_PAGEADDRSET, t); // HX8357D uses same registers!
#ifndef GM_USE_9341
    }
#endif // GM_USE_9341
    CS_IDLE;
}

// Unlike the 932X drivers that set the address window to the full screen
// by default (using the address counter for draw_px operations), the
// 7575 needs the address window set on all graphics operations.  In order
// to save a few register writes on each pixel drawn, the lower-right
// corner of the address window is reset after most fill operations, so
// that draw_px only needs to change the upper left each time.
void LCD::setLR(void)
{
    CS_ACTIVE;
    writeRegisterPair(HX8347G_COLADDREND_HI, HX8347G_COLADDREND_LO, _dimensions.x - 1);
    writeRegisterPair(HX8347G_ROWADDREND_HI, HX8347G_ROWADDREND_LO, _dimensions.y - 1);
    CS_IDLE;
}

// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
void LCD::flood(uint16_t color, uint32_t len)
{
    uint16_t blocks;
    uint8_t i, hi = color >> 8,
               lo = color;

    CS_ACTIVE;
    CD_COMMAND;
#ifndef GM_USE_9341
    if (driver == ID_9341)
    {
#endif // GM_USE_9341
        write8(0x2C);
#ifndef GM_USE_9341
    }
    else if (driver == ID_932X)
    {
        write8(0x00); // High byte of GRAM register...
        write8(0x22); // Write data to GRAM
    }
    else if (driver == ID_HX8357D)
    {
        write8(HX8357_RAMWR);
    }
    else
    {
        write8(0x22); // Write data to GRAM
    }
#endif // GM_USE_9341

    // Write first pixel normally, decrement counter by 1
    CD_DATA;
    write8(hi);
    write8(lo);
    len--;

    blocks = (uint16_t)(len / 64); // 64 pixels/block
    if (hi == lo)
    {
        // High and low bytes are identical.  Leave prior data
        // on the port(s) and just toggle the write strobe.
        while (blocks--)
        {
            i = 16; // 64 pixels/block / 4 pixels/pass
            do
            {
                WR_STROBE;
                WR_STROBE;
                WR_STROBE;
                WR_STROBE; // 2 bytes/pixel
                WR_STROBE;
                WR_STROBE;
                WR_STROBE;
                WR_STROBE; // x 4 pixels
            } while (--i);
        }
        // Fill any remaining pixels (1 to 64)
        for (i = (uint8_t)len & 63; i--;)
        {
            WR_STROBE;
            WR_STROBE;
        }
    }
    else
    {
        while (blocks--)
        {
            i = 16; // 64 pixels/block / 4 pixels/pass
            do
            {
                write8(hi);
                write8(lo);
                write8(hi);
                write8(lo);
                write8(hi);
                write8(lo);
                write8(hi);
                write8(lo);
            } while (--i);
        }
        for (i = (uint8_t)len & 63; i--;)
        {
            write8(hi);
            write8(lo);
        }
    }
    CS_IDLE;
}

void LCD::draw_px(int16_t x, int16_t y, color_t color)
{
    // clip
    if (x < 0 || y < 0 || x >= _dimensions.x || y >= _dimensions.y)
        return;

    CS_ACTIVE;
#ifndef GM_USE_9341
    if(driver == ID_932X)
    {
        int16_t t;
        switch (_rotation)
        {
        case 1:
            t = x;
            x = TFTWIDTH - 1 - y;
            y = t;
            break;
        case 2:
            x = TFTWIDTH - 1 - x;
            y = TFTHEIGHT - 1 - y;
            break;
        case 3:
            t = x;
            x = y;
            y = TFTHEIGHT - 1 - t;
            break;
        }
        writeRegister16(0x0020, x);
        writeRegister16(0x0021, y);
        writeRegister16(0x0022, color);
    }
    else if (driver == ID_7575)
    {
        uint8_t hi, lo;
        switch (_rotation)
        {
        default:
            lo = 0;
            break;
        case 1:
            lo = 0x60;
            break;
        case 2:
            lo = 0xc0;
            break;
        case 3:
            lo = 0xa0;
            break;
        }
        writeRegister8(HX8347G_MEMACCESS, lo);
        // Only upper-left is set -- bottom-right is full screen default
        writeRegisterPair(HX8347G_COLADDRSTART_HI, HX8347G_COLADDRSTART_LO, x);
        writeRegisterPair(HX8347G_ROWADDRSTART_HI, HX8347G_ROWADDRSTART_LO, y);
        hi = color >> 8;
        lo = color;
        CD_COMMAND;
        write8(0x22);
        CD_DATA;
        write8(hi);
        write8(lo);
    }
    else if ((driver == ID_9341) || (driver == ID_HX8357D))
    {
#endif // GM_USE_9341
        setAddrWindow(x, y, _dimensions.x - 1, _dimensions.y - 1);
        CS_ACTIVE;
        CD_COMMAND;
        write8(0x2C);
        CD_DATA;
        write8(color >> 8);
        write8(color);
#ifndef GM_USE_9341
    }
#endif // GM_USE_9341
    CS_IDLE;
}

void LCD::draw_h_line(int16_t x, int16_t y, int16_t w, color_t color)
{
    int16_t x2;

    // Initial off-screen clipping
    if (w <= 0 || y < 0 || y >= _dimensions.y || x >= _dimensions.x || (x2 = x + w - 1) < 0)
        return;

    if (x < 0) // Clip left
    {
        w += x;
        x = 0;
    }

    if (x2 >= _dimensions.x) // Clip right
    {
        x2 = _dimensions.x - 1;
        w = x2 - x + 1;
    }

    setAddrWindow(x, y, x2, y);
    flood(color, w);
    driver == ID_932X
        ? setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1)
        : setLR();
}

void LCD::draw_v_line(int16_t x, int16_t y, int16_t h, color_t color)
{
    int16_t y2;

    // Initial off-screen clipping
    if (h <= 0 || x < 0 || x >= _dimensions.x || y >= _dimensions.y || (y2 = y + h - 1) < 0)
        return;

    if (y < 0) // Clip top
    {
        h += y;
        y = 0;
    }

    if (y2 >= _dimensions.y) // Clip bottom
    {
        y2 = _dimensions.y - 1;
        h = y2 - y + 1;
    }

    setAddrWindow(x, y, x, y2);
    flood(color, h);
    driver == ID_932X
        ? setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1)
        : setLR();
}

void LCD::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    if (x0 == x1) {
        if (y0 > y1)
            std::swap(y0, y1);
        return draw_v_line(x0, y0, y1 - y0 + 1, color);
    } else if (y0 == y1) {
        if (x0 > x1)
            std::swap(x0, x1);
        return draw_h_line(x0, y0, x1 - x0 + 1, color);
    }

#if defined(ESP8266)
    yield();
#endif

    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep = y0 < y1 ? 1 : -1;

    for (; x0 <= x1; x0++)
    {
        steep
            ? draw_px(y0, x0, color)
            : draw_px(x0, y0, color);

        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void LCD::draw_bitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, color_t color)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t b = 0;

    for (int16_t j = 0; j < h; j++, y++)
    {
        for (int16_t i = 0; i < w; i++)
        {
            if (i & 7)
                b <<= 1;
            else
                b = bitmap[j * byteWidth + i / 8];

            if (b & 0x80)
            {
                draw_px(x + i, y, color);
            }
        }
    }
}

void LCD::fill_rect(int16_t x1, int16_t y1, int16_t w, int16_t h, color_t color) {
    int16_t  x2, y2;

    // Initial off-screen clipping
    if (w <= 0 || h <= 0 || x1 >= _dimensions.x || y1 >= _dimensions.y || (x2 = x1 + w - 1) < 0 || (y2 = y1 + h - 1) < 0)
        return;

    if (x1 < 0) // Clip left
    {
        w += x1;
        x1 = 0;
    }

    if (y1 < 0) // Clip top
    {
        h += y1;
        y1 = 0;
    }

    if (x2 >= _dimensions.x) // Clip right
    {
        x2 = _dimensions.x - 1;
        w = x2 - x1 + 1;
    }

    if (y2 >= _dimensions.y) // Clip bottom
    {
        y2 = _dimensions.y - 1;
        h = y2 - y1 + 1;
    }

    setAddrWindow(x1, y1, x2, y2);
    flood(color, (uint32_t)w * (uint32_t)h);
    driver == ID_932X
        ? setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1)
        : setLR();
}

// instead of drawing to screen draw line to a w * h dimensioned bitmap
// this is used to reduce the time inbetween draws by doing the calculations upfront
void LCD::draw_line_to_bitmap(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t *bitmap, int16_t w, int16_t h)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep = y0 < y1 ? 1 : -1;

    for (; x0 <= x1; x0++)
    {
        Pixel px = steep ? Pixel{ y0, x0 } : Pixel{ x0, y0 }; 
        if (px.x < w && px.y < h)
        {
            bitmap[px.y * byteWidth + px.x / 8] |= 0x80 >> (px.x % 8);
        }

        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void LCD::fill_screen(color_t color)
{

    if (driver == ID_932X)
    {

        // For the 932X, a full-screen address window is already the default
        // state, just need to set the address pointer to the top-left corner.
        // Although we could fill in any direction, the code uses the current
        // screen rotation because some users find it disconcerting when a
        // fill does not occur top-to-bottom.
        uint16_t x, y;
        switch (_rotation)
        {
        default:
            x = 0;
            y = 0;
            break;
        case 1:
            x = TFTWIDTH - 1;
            y = 0;
            break;
        case 2:
            x = TFTWIDTH - 1;
            y = TFTHEIGHT - 1;
            break;
        case 3:
            x = 0;
            y = TFTHEIGHT - 1;
            break;
        }
        CS_ACTIVE;
        writeRegister16(0x0020, x);
        writeRegister16(0x0021, y);
    } else if ((driver == ID_9341) || (driver == ID_7575) || (driver == ID_HX8357D)) {
        // For these, there is no settable address pointer, instead the
        // address window must be set for each drawing operation.  However,
        // this display takes rotation into account for the parameters, no
        // need to do extra rotation math here.
        setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1);
    }
    flood(color, (long)TFTWIDTH * (long)TFTHEIGHT);
}

void LCD::set_rotation(rotation_t x)
{
    _rotation = (x & 3);

    switch (_rotation)
    {
    case 0:
    case 2:
        _dimensions.x = WIDTH;
        _dimensions.y = HEIGHT;
        break;
    case 1:
    case 3:
        _dimensions.x = HEIGHT;
        _dimensions.y = WIDTH;
        break;
    }

    CS_ACTIVE;
    if (driver == ID_932X)
    {
        uint16_t t;
        switch (_rotation)
        {
        default:
            t = 0x1030;
            break;
        case 1:
            t = 0x1028;
            break;
        case 2:
            t = 0x1000;
            break;
        case 3:
            t = 0x1018;
            break;
        }
        writeRegister16(0x0003, t); // MADCTL
        // For 932X, init default full-screen address window:
        setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1); // CS_IDLE happens here
    }
    if (driver == ID_7575)
    {
        uint8_t t;
        switch (_rotation)
        {
        default:
            t = 0;
            break;
        case 1:
            t = 0x60;
            break;
        case 2:
            t = 0xc0;
            break;
        case 3:
            t = 0xa0;
            break;
        }
        writeRegister8(HX8347G_MEMACCESS, t);
        // 7575 has to set the address window on most drawing operations.
        // draw_px() cheats by setting only the top left...by default,
        // the lower right is always reset to the corner.
        setLR(); // CS_IDLE happens here
    }

    if (driver == ID_9341)
    {
        // MEME, HX8357D uses same registers as 9341 but different values
        uint16_t t;

        switch (_rotation)
        {
        case 2:
            t = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
            break;
        case 3:
            t = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            break;
        case 0:
            t = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
            break;
        case 1:
            t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            break;
        }
        writeRegister8(ILI9341_MADCTL, t); // MADCTL
        // For 9341, init default full-screen address window:
        setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1); // CS_IDLE happens here
    }

    if (driver == ID_HX8357D)
    {
        // MEME, HX8357D uses same registers as 9341 but different values
        uint16_t t;

        switch (_rotation)
        {
        case 2:
            t = HX8357B_MADCTL_RGB;
            break;
        case 3:
            t = HX8357B_MADCTL_MX | HX8357B_MADCTL_MV | HX8357B_MADCTL_RGB;
            break;
        case 0:
            t = HX8357B_MADCTL_MX | HX8357B_MADCTL_MY | HX8357B_MADCTL_RGB;
            break;
        case 1:
            t = HX8357B_MADCTL_MY | HX8357B_MADCTL_MV | HX8357B_MADCTL_RGB;
            break;
        }
        writeRegister8(ILI9341_MADCTL, t); // MADCTL
        // For 8357, init default full-screen address window:
        setAddrWindow(0, 0, _dimensions.x - 1, _dimensions.y - 1); // CS_IDLE happens here
    }
}

void LCD::draw_char(int16_t x, int16_t y, unsigned char c, color_t color, uint16_t bg, uint8_t size)
{
    draw_char(x, y, c, color, bg, size, size);
}

void LCD::draw_char(int16_t x, int16_t y, unsigned char c, color_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
{

    if (x >= _dimensions.x || y >= _dimensions.y || (x + 6 * size_x - 1) < 0 || (y + 8 * size_y - 1) < 0)
        return;

    for (int8_t i = 0; i < 5; i++) // Char bitmap = 5 columns
    {
        uint8_t line = pgm_read_byte(&gm_font[c * 5 + i]);
        for (int8_t j = 0; j < 8; j++, line >>= 1)
        {
            if (line & 1)
            {
                if (size_x == 1 && size_y == 1)
                    draw_px(x + i, y + j, color);
                else
                    fill_rect(x + i * size_x, y + j * size_y, size_x, size_y, color);
            } else if (bg != color) {
                if (size_x == 1 && size_y == 1)
                    draw_px(x + i, y + j, bg);
                else
                    fill_rect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
            }
        }
    }
    if (bg != color) // If opaque, draw vertical line for last column
    {
        if (size_x == 1 && size_y == 1)
            draw_v_line(x + 5, y, 8, bg);
        else
            fill_rect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
    }
}

void LCD::print_centered(const char *str, int16_t h)
{
    auto size = get_text_dimensions(str);
    set_cursor(_dimensions.x / 2 - size.x / 2, h);
    print(str);
}

void LCD::print_centered(const String &str, int16_t h)
{
    print_centered(str.c_str(), h);
}

void LCD::get_text_bounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
    uint8_t c;                                                  // Current character
    int16_t minx = 0x7FFF, miny = 0x7FFF, maxx = -1, maxy = -1; // Bound rect
    // Bound rect is intentionally initialized inverted, so 1st char sets it

    *x1 = x; // Initial position is value passed in
    *y1 = y;
    *w = *h = 0; // Initial size is zero

    while ((c = *str++))
    {
        // charBounds() modifies x/y to advance for each character,
        // and min/max x/y are updated to incrementally build bounding rect.
        char_bounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
    }

    if (maxx >= minx)
    {                         // If legit string bounds were found...
        *x1 = minx;           // Update x1 to least X coord,
        *w = maxx - minx + 1; // And w to bound rect width
    }
    if (maxy >= miny)
    { // Same for height
        *y1 = miny;
        *h = maxy - miny + 1;
    }
}

void LCD::get_text_bounds(const __FlashStringHelper *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
    uint8_t *s = (uint8_t *)str, c;

    *x1 = x;
    *y1 = y;
    *w = *h = 0;

    int16_t minx = _dimensions.x, miny = _dimensions.y, maxx = -1, maxy = -1;

    while ((c = pgm_read_byte(s++)))
        char_bounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if (maxx >= minx)
    {
        *x1 = minx;
        *w = maxx - minx + 1;
    }
    if (maxy >= miny)
    {
        *y1 = miny;
        *h = maxy - miny + 1;
    }
}

void LCD::get_text_bounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
    if (str.length() != 0)
        get_text_bounds(str.c_str(), x, y, x1, y1, w, h);
}

Size LCD::get_text_dimensions(const char *str)
{
    int16_t x, y;
    uint16_t w, h;
    get_text_bounds(str, 0, 0, &x, &y, &w, &h);
    return { w, h };
}

Size LCD::get_text_dimensions(const String &str)
{
    int16_t x, y;
    uint16_t w, h;
    get_text_bounds(str.c_str(), 0, 0, &x, &y, &w, &h);
    return { w, h };
}

void LCD::set_text_size(uint8_t s)
{
    set_text_size(s, s);
}

void LCD::set_text_size(uint8_t s_x, uint8_t s_y)
{
    _text_size.x = (s_x > 0) ? s_x : 1;
    _text_size.y = (s_y > 0) ? s_y : 1;
}

size_t LCD::write(uint8_t c)
{
    if (c == '\n')
    {
        _cursor.x = 0;
        _cursor.y += _text_size.y * 8; // advance y one line
    }
    else if (c != '\r')
    {
        if (_wrap && ((_cursor.x + _text_size.x * 6) > _dimensions.x)) // Off right?
        { 
            _cursor.x = 0;
            _cursor.y += _text_size.y * 8; // advance y one line
        }
        draw_char(_cursor.x, _cursor.y, c, _text_fg, _text_bg, _text_size.x, _text_size.y);
        _cursor.x += _text_size.x * 6; // Advance x one char
    }
    return 1;
}

void LCD::char_bounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy)
{
    if (c == '\n')
    {
        *x = 0;
        *y += _text_size.y * 8; // advance y one line
    }
    else if (c != '\r') // Normal char; ignore carriage returns
    {
        if (_wrap && ((*x + _text_size.x * 6) > _dimensions.x)) // Off right?
        {
            *x = 0;
            *y += _text_size.y * 8; // advance y one line
        }
        int x2 = *x + _text_size.x * 6 - 1, // Lower-right pixel of char
            y2 = *y + _text_size.y * 8 - 1;
        if (x2 > *maxx)
            *maxx = x2; // Track max x, y
        if (y2 > *maxy)
            *maxy = y2;
        if (*x < *minx)
            *minx = *x; // Track min x, y
        if (*y < *miny)
            *miny = *y;
        *x += _text_size.x * 6; // Advance x one char
    }
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
color_t LCD::color_rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// For I/O macros that were left undefined, declare function
// versions that reference the inline macros just once:

#ifndef write8
void LCD::write8(uint8_t value)
{
    write8inline(value);
}
#endif

#ifdef read8isFunctionalized
uint8_t LCD::read8fn(void)
{
    uint8_t result;
    read8inline(result);
    return result;
}
#endif

#ifndef setWriteDir
void LCD::setWriteDir(void)
{
    setWriteDirInline();
}
#endif

#ifndef setReadDir
void LCD::setReadDir(void)
{
    setReadDirInline();
}
#endif

#ifndef writeRegister8
void LCD::writeRegister8(uint8_t a, uint8_t d)
{
    writeRegister8inline(a, d);
}
#endif

#ifndef writeRegister16
void LCD::writeRegister16(uint16_t a, uint16_t d)
{
    writeRegister16inline(a, d);
}
#endif

#ifndef writeRegisterPair
void LCD::writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d)
{
    writeRegisterPairInline(aH, aL, d);
}
#endif

void LCD::writeRegister24(uint8_t r, uint32_t d)
{
    CS_ACTIVE;
    CD_COMMAND;
    write8(r);
    CD_DATA;
    delayMicroseconds(10);
    write8(d >> 16);
    delayMicroseconds(10);
    write8(d >> 8);
    delayMicroseconds(10);
    write8(d);
    CS_IDLE;
}

void LCD::writeRegister32(uint8_t r, uint32_t d)
{
    CS_ACTIVE;
    CD_COMMAND;
    write8(r);
    CD_DATA;
    delayMicroseconds(10);
    write8(d >> 24);
    delayMicroseconds(10);
    write8(d >> 16);
    delayMicroseconds(10);
    write8(d >> 8);
    delayMicroseconds(10);
    write8(d);
    CS_IDLE;
}

} // namespace gm::gfx