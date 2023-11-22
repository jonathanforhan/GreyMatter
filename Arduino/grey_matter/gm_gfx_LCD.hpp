#pragma once
#include "Arduino.h"
#include "Print.h"
#include "gm_gfx_Font.hpp"
#include "gm_gfx_Vec2.hpp"

namespace gm::gui {
    /// @private forward decl
    class GUI;
}

namespace gm::gfx {

/// @brief color type
using color_t = uint16_t;

/// @brief Color indicator for input to LCD
enum Color : color_t
{
    Black = 0x0000,
    Blue = 0x001F,
    Red = 0xF800,
    Green = 0x07E0,
    Cyan = 0x07FF,
    Magenta = 0xF81F,
    Yellow = 0xFFE0,
    White = 0xFFFF,
};

/// @brief rotation type
using rotation_t = uint8_t;

/// @brief predefined rotations in 90 deg increments
enum Rotation : rotation_t
{
    r0 = 0,
    r90 = 1,
    r180 = 2,
    r270 = 3,
};

/// @brief LCD class for drawing to TFTLCD
///
/// Extends the native Arduino `Print` class for easy IO operations
class LCD : public Print
{
    /// @brief Construct an LCD object
    /// @param cs Card Detect
    /// @param cd Chip Select
    /// @param wr Write
    /// @param rd Read
    /// @param rst Reset
    /// private onconstuctor ie can ONLY be constructed by GUI friend class
    LCD(uint8_t cs = A3, uint8_t cd = A2, uint8_t wr = A1, uint8_t rd = A0, uint8_t rst = A4);
    friend class gm::gui::GUI;

public:
    // Rule of 5, if you don't know this brush up on your C++
    LCD(const LCD &) = delete;            ///< non-copyable
    void operator=(const LCD &) = delete; ///< non-copyable
    LCD(LCD &&) = default;                ///< moveable
    LCD &operator=(LCD &&) = default;     ///< moveable

    /// @brief Begin the LCD display
    /// @param id ID of the LCD Driver
    void begin(uint16_t id = 0x9341);

    /// @brief clear the internal memory of the LCD
    void reset();

    /// @brief Sets the LCD address window (and address counter, on 932X).
    /// @param x1 X coord
    /// @param y1 Y coord
    /// @param x2 X coord
    /// @param y2 Y coord
    /// Relevant to rect/screen fills and H/V lines.  Input coordinates are
    /// assumed pre-sorted (e.g. x2 >= x1).
    void setAddrWindow(int x1, int y1, int x2, int y2);

    /// Unlike the 932X drivers that set the address window to the full screen
    /// by default (using the address counter for draw_px operations), the
    /// 7575 needs the address window set on all graphics operations.  In order
    /// to save a few register writes on each pixel drawn, the lower-right
    /// corner of the address window is reset after most fill operations, so
    /// that draw_px only needs to change the upper left each time.
    void setLR();

    /// @brief Fast block fill operation for fillScreen, fillRect, H/V line, etc.
    /// @param color Color to fille
    /// @param len bounds, len is inclusive, MUST be >= 1.
    void flood(uint16_t color, uint32_t len);

    /// @brief Draw a single pixel to screen
    /// @param x X coord 0-W
    /// @param y Y coord 0-H
    /// @param color Color of pixel
    void draw_px(int16_t x, int16_t y, color_t color);

    /// @brief Draw a horizontal line to screen
    /// @param x Starting X coord
    /// @param y Starting Y coord
    /// @param w Width of line
    /// @param color Color or line
    /// draw_h_line is an optimization and should be preferred compared to drawing pixels in a loop or draw_line
    void draw_h_line(int16_t x, int16_t y, int16_t w, color_t color);

    /// @brief Draw a vertical line to screen
    /// @param x Starting X coord
    /// @param y Starting Y coord
    /// @param h Height of line
    /// @param color Color or line
    /// draw_v_line is an optimization and should be preferred compared to drawing pixels in a loop or draw_line
    void draw_v_line(int16_t x, int16_t y, int16_t h, color_t color);

    /// @brief Draw a line to the screen
    /// @param x0 Statring X coord
    /// @param y0 Starting Y coord
    /// @param x1 Ending X coord
    /// @param y1 Ending Y coord
    /// @param color Color of line
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);

    /// @brief Draw a bitmap to screen
    /// @param x Starting X coord
    /// @param y Starting Y coord
    /// @param bitmap Bitmap byte pointer
    /// @param w Width of bitmap in bits
    /// @param h Height of bitmap in bits
    /// @param color
    void draw_bitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, color_t color);

    /// @brief Fill a rectangle on the screen
    /// @param x Starting X coord
    /// @param y Starting Y coord
    /// @param w Width
    /// @param h Height
    /// @param color Rectangle Color
    void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, color_t color);

    /// @brief Draw a line to a bitmap
    /// @param x0 Starting X coord of line
    /// @param y0 Starting Y coord of line
    /// @param x1 Ending X coord of line
    /// @param y1 Ending Y coord of line
    /// @param bitmap Bitmap pointer
    /// @param w Width of bitmap in bits
    /// @param h Height of bitmap in bits
    /// This function allows you to use a bitmap as a back buffer and do all the calcuations BEFORE draw, an perceived optimization
    void draw_line_to_bitmap(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t *bitmap, int16_t w, int16_t h);

    /// @brief Draw pixel to screen
    /// @param px pixel position
    /// @param color color of pixel
    void draw_px(Pixel px, color_t color) { draw_px(px.x, px.y, color); }

    /// @brief Draw horizonatal line to the screen
    /// @param px pixel position
    /// @param w width of line
    /// @param color color of pixel
    /// draw_h_line is an optimization, prefer it over other methods when drawing horizontal lines
    void draw_h_line(Pixel px, int16_t w, color_t color) { draw_h_line(px.x, px.y, w, color); }

    /// @brief Draw vertical line to the screen
    /// @param px pixel position
    /// @param h height on line
    /// @param color color of line
    /// draw_v_line is an optimization, prefer it over other methods when drawing vertical lines
    void draw_v_line(Pixel px, int16_t h, color_t color) { draw_v_line(px.x, px.y, h, color); }

    /// @brief Draw a line to the screen
    /// @param src source pixel of line
    /// @param dst destination pixel of line
    /// @param color color of pixel
    void draw_line(Pixel src, Pixel dst, color_t color) { draw_line(src.x, src.y, dst.x, dst.y, color); }

    /// @brief Fill rectangle on screen
    /// @param px stating pixel
    /// @param size size of rectangle
    /// @param color color of rectangle
    void fill_rect(Pixel px, Size size, color_t color) { fill_rect(px.x, px.y, size.x, size.y, color); }

    /// @brief Draw a bitmap to the screen
    /// @param px starting pixel on the screen
    /// @param bitmap bitmap pointer
    /// @param size size of bitmap in bits
    /// @param color color of to draw
    void draw_bitmap(Pixel px, uint8_t *bitmap, Size size, color_t color) { draw_bitmap(px.x, px.y, bitmap, size.x, size.y, color); }

    /// @brief Draw a line to a bitmap
    /// @param src source pixel on bitmap to draw line
    /// @param dst destination pixel on line
    /// @param bitmap Bitmap pointer
    /// @param size size of bitmap in bits
    /// This function allows you to use a bitmap as a back buffer and do all the calcuations BEFORE draw, an perceived optimization
    void draw_line_to_bitmap(Pixel src, Pixel dst, uint8_t *bitmap, Size size)
    {
        draw_line_to_bitmap(src.x, src.y, dst.x, dst.y, bitmap, size.x, size.y);
    }

    /// @brief File the screen with a color
    /// @param color color to fill
    /// @warning this is a slow draw call
    void fill_screen(color_t color);

    /// @brief Set the ratation of the screen [0|90|180|270]
    /// @param rotation rotation enum class
    void set_rotation(rotation_t rotation);

    /// @brief Draw a character to the screen
    /// @param x starting x coord
    /// @param y starting y coord
    /// @param c character to draw
    /// @param color color of char
    /// @param bg background color
    /// @param size size to draw (scalar)
    void draw_char(int16_t x, int16_t y, unsigned char c, color_t color, uint16_t bg, uint8_t size);

    /// @brief Draw a character to the screen
    /// @param x starting x coord
    /// @param y starting y coord
    /// @param c character to draw
    /// @param color color of char
    /// @param bg background color
    /// @param size_x size to draw in x direction
    /// @param size_y  size to draw in y direction
    void draw_char(int16_t x, int16_t y, unsigned char c, color_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);

    /// @brief Print text centered on the screen
    /// @param str string to print
    /// @param h height at which to print
    void print_centered(const char *str, int16_t h);

    /// @brief Print text centered on the screen
    /// @param str string to print
    /// @param h height at which to print
    void print_centered(const String &str, int16_t h);

    /// @brief Get the bounds of a string of text
    /// @param str string of text
    /// @param x starting x coord
    /// @param y starting y coord
    /// @param x1 OUTPUT ending x coord
    /// @param y1 OUTPUT ending y coord
    /// @param w OUTPUT width
    /// @param h OUTPUT height
    void get_text_bounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

    /// @brief Get the bounds of a string of text
    /// @param str string of text
    /// @param x starting x coord
    /// @param y starting y coord
    /// @param x1 OUTPUT ending x coord
    /// @param y1 OUTPUT ending y coord
    /// @param w OUTPUT width
    /// @param h OUTPUT height
    void get_text_bounds(const __FlashStringHelper *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

    /// @brief Get the bounds of a string of text
    /// @param str string of text
    /// @param x starting x coord
    /// @param y starting y coord
    /// @param x1 OUTPUT ending x coord
    /// @param y1 OUTPUT ending y coord
    /// @param w OUTPUT width
    /// @param h OUTPUT height
    void get_text_bounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

    /// @brief Simplified version of get_text_bounds
    /// @param str string to get dimensions of
    /// @return the size needed to hold string
    Size get_text_dimensions(const char *str);

    /// @brief Simplified version of get_text_bounds
    /// @param str string to get dimensions of
    /// @return the size needed to hold string
    Size get_text_dimensions(const String &str);

    /// @brief Set the global size of text being printed
    /// @param s size
    void set_text_size(uint8_t s);

    /// @brief Set the global size of text being printed
    /// @param sx size x
    /// @param sy size y
    void set_text_size(uint8_t sx, uint8_t sy);

    using Print::write;

    /// @brief overloaded write function extending Print class
    /// @param  c to write
    /// @return size written
    size_t write(uint8_t c) override;

    /// @brief set the cursor position
    /// @param x x coord
    /// @param y y coord
    void set_cursor(int16_t x, int16_t y)
    {
        _cursor.x = x;
        _cursor.y = y;
    }

    /// @brief set the cursor position
    /// @param pos position of cursor
    void set_cursor(Position pos)
    {
        _cursor.x = pos.x;
        _cursor.y = pos.y;
    }

    /// @brief Set global text color
    /// @param c color
    void set_text_color(color_t c) { _text_fg = c; }

    /// @brief Set global text color
    /// @param c color
    /// @param bg background color
    void set_text_color(color_t c, color_t bg)
    {
        _text_fg = c;
        _text_bg = bg;
    }

    /// @brief Set global text-wrap
    /// @param b t/f
    void set_text_wrap(bool b = true) { _wrap = b; }

    /// @brief Width getter
    /// @return X dimension of screen
    int16_t width() const { return _dimensions.x; };

    /// @brief Height getter
    /// @return Y dimension of screen
    int16_t height() const { return _dimensions.y; }

    /// @brief Dimensions getter
    /// @return xy dimensions of screen
    Size dimensions() const { return _dimensions; }

    /// @brief Rotation getter
    /// @return rotation of screen
    uint8_t rotation() const { return _rotation; }

    /// @brief Cursor X position getter
    /// @return X coord of cursor
    int16_t cursor_x() const { return _cursor.x; }

    /// @brief Cursor Y position getter
    /// @return Y coord of cursor
    int16_t cursor_y() const { return _cursor.y; };

    /// @brief Cursor position getter
    /// @return position of cursor
    Position cursor() const { return _cursor; };

    /// @brief Convert your RGB to a color type
    /// @param r red
    /// @param g green
    /// @param b blue
    /// @return color type
    color_t color_rgb(uint8_t r, uint8_t g, uint8_t b);

protected:
    /// @brief Get the bounding box for a char
    /// @param c 
    /// @param x 
    /// @param y 
    /// @param minx 
    /// @param miny 
    /// @param maxx 
    /// @param maxy 
    void char_bounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);

    int16_t WIDTH;        ///< @brief This is the 'raw' display width - never changes
    int16_t HEIGHT;       ///< @brief This is the 'raw' display height - never changes
    Size _dimensions;     ///< @brief Display dimensions as modified by current rotation
    Position _cursor;     ///< @brief cursor position
    color_t _text_fg;     ///< @brief text foreground
    color_t _text_bg;     ///< @brief text background
    Size _text_size;      ///< @brief text size
    rotation_t _rotation; ///< @brief text rotation
    bool _wrap;           ///< @brief text wrap

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