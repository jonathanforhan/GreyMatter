#pragma once
#include "Arduino.h"
#include "Print.h"
#include "gm_gfx_Font.hpp"
#include "gm_gfx_Vec2.hpp"

namespace gm::gfx {

using color_t = uint16_t;
enum Color : color_t
{
    Black   = 0x0000,
    Blue    = 0x001F,
    Red     = 0xF800,
    Green   = 0x07E0,
    Cyan    = 0x07FF,
    Magenta = 0xF81F,
    Yellow  = 0xFFE0,
    White   = 0xFFFF,
};

using rotation_t = uint8_t;
enum Rotation : rotation_t
{
    r0      = 0,
    r90     = 1,
    r180    = 2,
    r270    = 3,
};

class LCD : public Print
{
public:
    LCD(uint8_t cs = A3, uint8_t cd = A2, uint8_t wr = A1, uint8_t rd = A0, uint8_t rst = A4);

    void begin(uint16_t id = 0x9341);
    void reset();
    void setAddrWindow(int x1, int y1, int x2, int y2);
    void setLR();
    void flood(uint16_t color, uint32_t len);

    void draw_px(int16_t x, int16_t y, color_t color);
    void draw_h_line(int16_t x, int16_t y, int16_t w, color_t color);
    void draw_v_line(int16_t x, int16_t y, int16_t h, color_t color);
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);
    void draw_bitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, color_t color);
    void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, color_t color);
    void draw_line_to_bitmap(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t *bitmap, int16_t w, int16_t h);

    void draw_px(Pixel px, color_t color)                  { draw_px(px.x, px.y, color); }
    void draw_h_line(Pixel px, int16_t w, color_t color)   { draw_h_line(px.x, px.y, w, color); }
    void draw_v_line(Pixel px, int16_t h, color_t color)   { draw_v_line(px.x, px.y, h, color); }
    void draw_line(Pixel src, Pixel dst, color_t color)    { draw_line(src.x, src.y, dst.x, dst.y, color); }
    void fill_rect(Pixel px, Size size, color_t color)     { fill_rect(px.x, px.y, size.x, size.y, color); }
    void draw_bitmap(Pixel px, uint8_t *bitmap, Size size, color_t color)
    { draw_bitmap(px.x, px.y, bitmap, size.x, size.y, color); }
    void draw_line_to_bitmap(Pixel src, Pixel dst, uint8_t *bitmap, Size size)
    { draw_line_to_bitmap(src.x, src.y, dst.x, dst.y, bitmap, size.x, size.y); }

    void fill_screen(color_t color);
    void set_rotation(rotation_t rotation);
    void draw_char(int16_t x, int16_t y, unsigned char c, color_t color, uint16_t bg, uint8_t size);
    void draw_char(int16_t x, int16_t y, unsigned char c, color_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    void print_centered(const char *str, int16_t h);
    void print_centered(const String &str, int16_t h);
    void get_text_bounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void get_text_bounds(const __FlashStringHelper *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void get_text_bounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    Size get_text_dimensions(const char *str);
    Size get_text_dimensions(const String &str);
    void set_text_size(uint8_t s);
    void set_text_size(uint8_t sx, uint8_t sy);
    using Print::write;
    size_t write(uint8_t) override;

    void set_cursor(int16_t x, int16_t y)   { _cursor.x = x; _cursor.y = y; }
    void set_cursor(Position pos)           { _cursor.x = pos.x; _cursor.y = pos.y; }
    void set_text_color(color_t c)          { _text_fg = c; }
    void set_text_color(color_t c, color_t bg) { _text_fg = c; _text_bg = bg; }
    void set_text_wrap(bool b = true)       { _wrap = b; }

    int16_t width() const       { return _dimensions.x; };
    int16_t height() const      { return _dimensions.y; }
    Size dimensions() const     { return _dimensions; }
    uint8_t rotation() const    { return _rotation; }
    int16_t cursor_x() const    { return _cursor.x; }
    int16_t cursor_y() const    { return _cursor.y; };
    Size cursor() const         { return _cursor; };

    color_t color_rgb(uint8_t r, uint8_t g, uint8_t b);

protected:
    void char_bounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);

    int16_t WIDTH;        // This is the 'raw' display width - never changes
    int16_t HEIGHT;       // This is the 'raw' display height - never changes
    Size _dimensions;     // Display dimensions as modified by current rotation
    Position _cursor;
    color_t _text_fg;
    color_t _text_bg;
    Size _text_size;
    rotation_t _rotation;
    bool _wrap;

private:
#ifndef write8
    void write8(uint8_t value);
#endif
#ifndef setWriteDir
    void setWriteDir(void);
#endif
#ifndef setReadDir
    void setReadDir(void);
#endif
#ifndef writeRegister8
    void writeRegister8(uint8_t a, uint8_t d);
#endif
#ifndef writeRegister16
    void writeRegister16(uint16_t a, uint16_t d);
#endif
    void writeRegister24(uint8_t a, uint32_t d);
    void writeRegister32(uint8_t a, uint32_t d);
#ifndef writeRegisterPair
    void writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d);
#endif
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

} // namespace gm::gfx