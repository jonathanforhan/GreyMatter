#pragma once

/// @file gm_gfx_Font.hpp
/// @brief Provides the structure for the font reprentation in GM Embedded

namespace gm::gfx {

/// @brief Font data stored PER GLYPH
struct Glyph
{
    uint16_t bitmap_offset; ///< @brief Pointer into gm::gfx::font->bitmap
    uint8_t width;          ///< @brief Bitmap dimensions in pixels
    uint8_t height;         ///< @brief Bitmap dimensions in pixels
    uint8_t x_advance;      ///< @brief Distance to advance cursor (x axis)
    int8_t x_offset;        ///< @brief X dist from cursor pos to UL corner
    int8_t y_offset;        ///< @brief Y dist from cursor pos to UL corner
};

/// @brief Data stored for FONT AS A WHOLE
struct Font
{
    uint8_t *bitmap;   ///< @brief Glyph bitmaps, concatenated
    Glyph *glyph;      ///< @brief Glyph array
    uint16_t first;    ///< @brief ASCII extents (first char)
    uint16_t last;     ///< @brief ASCII extents (last char)
    uint8_t y_advance; ///< @brief Newline distance (y axis)
};

} // namespace gm::gfx
