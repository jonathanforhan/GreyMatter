#pragma once
#include "gm_gfx_LCD.hpp"
#include "gm_gfx_Vec2.hpp"
#include "gm_gfx_Vec3.hpp"

namespace gm::gui {

class Cube
{
public:
    Cube(const gfx::LCD &lcd) : lcd(lcd) { reset(); }
    void rotate_x(float theta);
    void rotate_y(float theta);
    void rotate_z(float theta);
    void scale(gfx::Vec3<float> scale);
    void offset(gfx::Size offset);
    void draw(gfx::color_t color);
    void reset();

private:
    const gfx::Point3 cube[8] = {
            { -1, -1, -1 },
            {  1, -1, -1 },
            {  1,  1, -1 },
            { -1,  1, -1 },
            { -1, -1,  1 },
            {  1, -1,  1 },
            {  1,  1,  1 },
            { -1,  1,  1 },
    };
    const size_t indices[24] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            4, 5,
            5, 6,
            6, 7,
            7, 4,
            0, 4,
            1, 5,
            2, 6,
            3, 7,
    };
    gfx::Point3 verts[sizeof(cube) / sizeof(*cube)] = {};
    uint8_t bitmap[200] = {};
    uint8_t cache[200] = {};
    gfx::Size local_offset = { 0, 0 };
    float max_scale = 1;

    const gfx::LCD &lcd;
};

} // namespace gm::gui