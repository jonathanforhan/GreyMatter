#pragma once

namespace gm::gfx {

template <class T>
struct Vec2
{
    T x, y;

    Vec2 operator+(T n)                        { return { x + n, y + n }; }
    Vec2 operator-(T n)                        { return { x - n, y - n }; }
    Vec2 operator*(T n)                        { return { x * n, y * n }; }
    Vec2 operator/(T n)                        { return { x / n, y / n }; }
    Vec2 operator+(const Vec2 &v)              { return { x + v.x, y + v.y }; }
    Vec2 operator-(const Vec2 &v)              { return { x - v.x, y - v.y }; }
    Vec2 operator*(const Vec2 &v)              { return { x * v.x, y * v.y }; }
    Vec2 operator/(const Vec2 &v)              { return { x / v.x, y / v.y }; }
    Vec2 &operator+=(T n)                      { x += n; y += n; return *this; }
    Vec2 &operator-=(T n)                      { x -= n; y -= n; return *this; }
    Vec2 &operator*=(T n)                      { x *= n; y *= n; return *this; }
    Vec2 &operator/=(T n)                      { x /= n; y /= n; return *this; }
    Vec2 &operator+=(const Vec2 &v)            { x += v.x; y += v.y; return *this; }
    Vec2 &operator-=(const Vec2 &v)            { x -= v.x; y -= v.y; return *this; }
    Vec2 &operator*=(const Vec2 &v)            { x *= v.x; y *= v.y; return *this; }
    Vec2 &operator/=(const Vec2 &v)            { x /= v.x; y /= v.y; return *this; }
    constexpr bool operator==(const Vec2 &v)   { return (v.x == x && v.y == y); }
    constexpr bool operator!=(const Vec2 &v)   { return !(*this == v); }
};

using Point2 = Vec2<int16_t>;
using Pixel = Vec2<int16_t>;
using Size = Vec2<int16_t>;

} // namespace gm::gfx
