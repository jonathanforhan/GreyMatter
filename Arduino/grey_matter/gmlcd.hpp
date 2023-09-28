#pragma once
#if ARDUINO >= 100
#   include "Arduino.h"
#else
#   include "WProgram.h"
#endif
#include <Adafruit_GFX.h>

#include "gmlcd/Point.hpp"
#include "gmlcd/Size.hpp"

namespace gmlcd {
        
enum class Color : uint16_t {
    Black   = 0x0000,
    Blue    = 0x001F,
    Red     = 0xF800,
    Green   = 0x07E0,
    Cyan    = 0x07FF,
    Magenta = 0xF81F,
    Yellow  = 0xFFE0,
    White   = 0xFFFF,
};
        
class GMLCD : public Adafruit_GFX
{

public:
    GMLCD(uint8_t cs = A3, uint8_t cd = A2, uint8_t wr = A1, uint8_t rd = A0, uint8_t rst = A4);
    // GMLCD(void);

    void begin(uint16_t id = 0x9341);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
    void fillScreen(uint16_t color);
    void reset(void);
    void setRegisters8(uint8_t *ptr, uint8_t n);
    void setRegisters16(uint16_t *ptr, uint8_t n);
    void setRotation(uint8_t x);
    // These methods are public in order for BMP examples to work:
    void setAddrWindow(int x1, int y1, int x2, int y2);
    void pushColors(uint16_t *data, uint8_t len, boolean first);

    uint16_t color565(uint8_t r, uint8_t g, uint8_t b), readPixel(int16_t x, int16_t y), readID(void);
    uint32_t readReg(uint8_t r);

    // grey api
    constexpr void drawPixel(Point px, Color color)
    { drawPixel(px.x, px.y, (uint16_t)color); }

    constexpr void drawFastHLine(Point px, int16_t w, Color color)
    { drawFastHLine(px.x, px.y, w, (uint16_t)color); }

    constexpr void drawFastVLine(Point px, int16_t w, Color color)
    { drawFastVLine(px.x, px.y, w, (uint16_t)color); }

    constexpr void drawLine(Point src, Point dst, Color color)
    { Adafruit_GFX::drawLine(src.x, src.y, dst.x, dst.y, (uint16_t)color); }

    constexpr void fillRect(Point px, Size size, Color color)
    { fillRect(px.x, px.y, size.w, size.h, (uint16_t)color); }

    constexpr void fillScreen(Color color)
    { fillScreen((uint16_t)color); }

    constexpr void drawRect(Point px, Size size, Color color)
    { Adafruit_GFX::drawRect(px.x, px.y, size.w, size.h, (uint16_t)color); }

private:
    void init(),
    // These items may have previously been defined as macros
    // in pin_magic.h.  If not, function versions are declared:
#ifndef write8
        write8(uint8_t value),
#endif
#ifndef setWriteDir
        setWriteDir(void),
#endif
#ifndef setReadDir
        setReadDir(void),
#endif
#ifndef writeRegister8
        writeRegister8(uint8_t a, uint8_t d),
#endif
#ifndef writeRegister16
        writeRegister16(uint16_t a, uint16_t d),
#endif
        writeRegister24(uint8_t a, uint32_t d),
        writeRegister32(uint8_t a, uint32_t d),
#ifndef writeRegisterPair
        writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d),
#endif
        setLR(void),
        flood(uint16_t color, uint32_t len);
    uint8_t driver;

#ifndef read8
    uint8_t read8fn(void);
#define read8isFunctionalized
#endif

#ifndef USE_ADAFRUIT_SHIELD_PINOUT

#ifdef __AVR__
    volatile uint8_t *csPort, *cdPort, *wrPort, *rdPort;
    uint8_t csPinSet, cdPinSet, wrPinSet, rdPinSet, csPinUnset, cdPinUnset, wrPinUnset, rdPinUnset, _reset;
#endif
#if defined(__SAM3X8E__)
    Pio *csPort, *cdPort, *wrPort, *rdPort;
    uint32_t csPinSet, cdPinSet, wrPinSet, rdPinSet, csPinUnset, cdPinUnset, wrPinUnset, rdPinUnset, _reset;
#endif

#endif
};

// For compatibility with sketches written for older versions of library.
// Color function name was changed to 'color565' for parity with 2.2" LCD
// library.
#define Color565 color565

} // namespace gmlcd
