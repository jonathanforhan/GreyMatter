#pragma once

namespace gm::gfx {

/// @brief Vec2 container
/// @tparam T underlying datatype
template <class T>
struct Vec2
{
    T x; ///< @brief x position
    T y; ///< @brief y position

    Vec2 operator+(T n)                        { return { x + n, y + n }; }           ///< @private
    Vec2 operator-(T n)                        { return { x - n, y - n }; }           ///< @private
    Vec2 operator*(T n)                        { return { x * n, y * n }; }           ///< @private
    Vec2 operator/(T n)                        { return { x / n, y / n }; }           ///< @private
    Vec2 operator+(const Vec2 &v)              { return { x + v.x, y + v.y }; }       ///< @private
    Vec2 operator-(const Vec2 &v)              { return { x - v.x, y - v.y }; }       ///< @private
    Vec2 operator*(const Vec2 &v)              { return { x * v.x, y * v.y }; }       ///< @private
    Vec2 operator/(const Vec2 &v)              { return { x / v.x, y / v.y }; }       ///< @private
    Vec2 &operator+=(T n)                      { x += n; y += n; return *this; }      ///< @private
    Vec2 &operator-=(T n)                      { x -= n; y -= n; return *this; }      ///< @private
    Vec2 &operator*=(T n)                      { x *= n; y *= n; return *this; }      ///< @private
    Vec2 &operator/=(T n)                      { x /= n; y /= n; return *this; }      ///< @private
    Vec2 &operator+=(const Vec2 &v)            { x += v.x; y += v.y; return *this; }  ///< @private
    Vec2 &operator-=(const Vec2 &v)            { x -= v.x; y -= v.y; return *this; }  ///< @private
    Vec2 &operator*=(const Vec2 &v)            { x *= v.x; y *= v.y; return *this; }  ///< @private
    Vec2 &operator/=(const Vec2 &v)            { x /= v.x; y /= v.y; return *this; }  ///< @private
    constexpr bool operator==(const Vec2 &v)   { return (v.x == x && v.y == y); }     ///< @private
    constexpr bool operator!=(const Vec2 &v)   { return !(*this == v); }              ///< @private
};

using Point2 = Vec2<float>;
using Position = Vec2<int16_t>;
using Pixel = Vec2<int16_t>;
using Size = Vec2<int16_t>;

} // namespace gm::gfx
