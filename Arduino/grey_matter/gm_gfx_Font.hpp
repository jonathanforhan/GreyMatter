#pragma once

namespace gm::gfx {

// Font data stored PER GLYPH
typedef struct
{
    uint16_t bitmap_offset; //< Pointer into gm::gfx::font->bitmap
    uint8_t width;          //< Bitmap dimensions in pixels
    uint8_t height;         //< Bitmap dimensions in pixels
    uint8_t x_advance;      //< Distance to advance cursor (x axis)
    int8_t x_offset;        //< X dist from cursor pos to UL corner
    int8_t y_offset;        //< Y dist from cursor pos to UL corner
} Glyph;

// Data stored for FONT AS A WHOLE
typedef struct
{
    uint8_t *bitmap;   //< Glyph bitmaps, concatenated
    Glyph *glyph;      //< Glyph array
    uint16_t first;    //< ASCII extents (first char)
    uint16_t last;     //< ASCII extents (last char)
    uint8_t y_advance; //< Newline distance (y axis)
} Font;

} // namespace gm::gfx
