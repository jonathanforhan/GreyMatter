#include "gm_gui_Cube.hpp"
#define GML_STL 1
#include "gm_stl/gm_stl.hpp"

#define LEN(_X) (sizeof(_X) / sizeof(*_X))

namespace gm::gui {
using namespace gm::gfx;

void Cube::rotate_x(float theta)
{
    for (auto i = 0; i < LEN(verts); i++)
    {
        auto y_cpy = verts[i].y;
        auto z_cpy = verts[i].z;
        verts[i].y = y_cpy * cos(theta) - z_cpy * sin(theta);
        verts[i].z = y_cpy * sin(theta) + z_cpy * cos(theta);
    }
}            

void Cube::rotate_y(float theta)
{
    for (auto i = 0; i < LEN(verts); i++)
    {
        auto x_cpy = verts[i].x;
        auto z_cpy = verts[i].z;
        verts[i].x = x_cpy * cos(theta) - z_cpy * sin(theta);
        verts[i].z = x_cpy * sin(theta) + z_cpy * cos(theta);
    }
}            

void Cube::rotate_z(float theta)
{
    for (auto i = 0; i < LEN(verts); i++)
    {
        auto y_cpy = verts[i].y;
        auto z_cpy = verts[i].z;
        verts[i].y = y_cpy * cos(theta) - z_cpy * sin(theta);
        verts[i].z = y_cpy * sin(theta) + z_cpy * cos(theta);
    }
}

void Cube::scale(gfx::Vec3<float> scale)
{
    for (int i = 0; i < LEN(verts); i++)
    {
        verts[i] *= scale;
        verts[i] += scale + scale * 0.5;
        max_scale = std::max(max_scale, scale.x);
        max_scale = std::max(max_scale, scale.y);
        max_scale = std::max(max_scale, scale.z);
    }
}

void Cube::offset(Size offset)
{
    local_offset += offset;
}

void Cube::draw(color_t color)
{
    constexpr int xy = sqrt(LEN(bitmap) * 8);

    for (int i = 0; i < LEN(bitmap); i++)
    {
        cache[i] = bitmap[i];
        bitmap[i] = 0;
    }

    for (size_t i = 0; i < LEN(indices); i += 2)
    {
        Pixel src = { verts[indices[i]].x,   verts[indices[i]].y };
        Pixel dst = { verts[indices[i+1]].x, verts[indices[i+1]].y };
        lcd.draw_line_to_bitmap(src, dst, bitmap, { xy, xy });
    }

    // smooth rendering
    for (int i = 0; i < LEN(bitmap); i++)
    {
        cache[i] &= ~bitmap[i];
        bitmap[i] &= ~cache[i];
    }

    const Pixel position = local_offset - (max_scale + 0.5 * max_scale);

    // erase and redraw as close as possible
    lcd.draw_bitmap(position, bitmap, { xy, xy }, color);
    lcd.draw_bitmap(position, cache, { xy, xy }, gfx::Black);

    local_offset = { 0, 0 };
    reset();
}

void Cube::reset()
{
    for (int i = 0; i < LEN(verts); i++)
        verts[i] = cube[i];
}

} // namespace gm::gui