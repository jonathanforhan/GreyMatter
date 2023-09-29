#pragma once
#include "vec3.hpp"
#include "gmlcd/Size.hpp"
#include "gmlcd.hpp"

namespace gmlcd {

class Cube
{
public:
    Cube(const GMLCD &lcd) : lcd(lcd) { reset(); }
    void rotate_x(float theta);
    void rotate_y(float theta);
    void rotate_z(float theta);
    void scale(uint16_t scale);
    void offset(Size offset);
    void draw(Color color);
    void reset();

private:
    static constexpr int n_points = 8;
    const vec3 cube[n_points] = {
            { -1, -1, -1 },
            {  1, -1, -1 },
            {  1,  1, -1 },
            { -1,  1, -1 },
            { -1, -1,  1 },
            {  1, -1,  1 },
            {  1,  1,  1 },
            { -1,  1,  1 },
    };
    static constexpr int n_indices = 24;
    const size_t indices[n_indices] = {
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
    vec3 verts[n_points] = {};
    vec3 cache[n_points] = {};
    const GMLCD &lcd;
};

} // namespace gmlcd
