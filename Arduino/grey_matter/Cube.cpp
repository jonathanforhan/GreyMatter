#include "Cube.hpp"

namespace gmlcd {

void Cube::rotate_x(float theta)
{
    for (auto i = 0; i < n_points; i++)
    {
        auto y_cpy = verts[i].y;
        auto z_cpy = verts[i].z;
        verts[i].y = y_cpy * cos(theta) - z_cpy * sin(theta);
        verts[i].z = y_cpy * sin(theta) + z_cpy * cos(theta);
    }
}            

void Cube::rotate_y(float theta)
{
    for (auto i = 0; i < n_points; i++)
    {
        auto x_cpy = verts[i].x;
        auto z_cpy = verts[i].z;
        verts[i].x = x_cpy * cos(theta) - z_cpy * sin(theta);
        verts[i].z = x_cpy * sin(theta) + z_cpy * cos(theta);
    }
}            

void Cube::rotate_z(float theta)
{
    for (auto i = 0; i < n_points; i++)
    {
        auto y_cpy = verts[i].y;
        auto z_cpy = verts[i].z;
        verts[i].y = y_cpy * cos(theta) - z_cpy * sin(theta);
        verts[i].z = y_cpy * sin(theta) + z_cpy * cos(theta);
    }
}

void Cube::scale(uint16_t scale)
{
    for (int i = 0; i < n_points; i++)
    {
        verts[i].x *= scale;
        verts[i].y *= scale;
        verts[i].z *= scale;
    }
}

void Cube::offset(Size offset)
{
    for (int i = 0; i < n_points; i++)
    {
        verts[i].x += offset.w;
        verts[i].y += offset.h;
    }
}

void Cube::draw(Color color)
{
    for (size_t i = 0; i < n_indices; i += 2)
    {
        Point src = { cache[indices[i]].x,   cache[indices[i]].y };
        Point dst = { cache[indices[i+1]].x, cache[indices[i+1]].y };
        lcd.drawLine(src, dst, 0);
    }
    
    for (size_t i = 0; i < n_indices; i += 2)
    {
        Point src = { verts[indices[i]].x,   verts[indices[i]].y };
        Point dst = { verts[indices[i+1]].x, verts[indices[i+1]].y };
        lcd.drawLine(src, dst, color);
    }

    for (size_t i = 0; i < n_points; i++)
        cache[i] = verts[i];

    reset();
}

void Cube::reset()
{
    for (int i = 0; i < n_points; i++)
        verts[i] = cube[i];
}

} // namespace gmlcd