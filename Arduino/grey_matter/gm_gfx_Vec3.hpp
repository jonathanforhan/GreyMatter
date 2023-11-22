#pragma once

namespace gm::gfx {

/// @brief Vec3 container
/// @tparam T underlying datatype
template <class T>
struct Vec3
{
    T x; ///< @brief x position
    T y; ///< @brief y position
    T z; ///< @brief z position

    Vec3 operator+(T n)                        { return { x + n, y + n, z + n }; }              ///< @private
    Vec3 operator-(T n)                        { return { x - n, y - n, z - n }; }              ///< @private
    Vec3 operator*(T n)                        { return { x * n, y * n, z * n }; }              ///< @private
    Vec3 operator/(T n)                        { return { x / n, y / n, z / n }; }              ///< @private
    Vec3 operator+(const Vec3 &v)              { return { x + v.x, y + v.y, z + v.z }; }        ///< @private
    Vec3 operator-(const Vec3 &v)              { return { x - v.x, y - v.y, z - v.z }; }        ///< @private
    Vec3 operator*(const Vec3 &v)              { return { x * v.x, y * v.y, z * v.z }; }        ///< @private
    Vec3 operator/(const Vec3 &v)              { return { x / v.x, y / v.y, z / v.z }; }        ///< @private
    Vec3 &operator+=(T n)                      { x += n; y += n; z += n; return *this; }        ///< @private
    Vec3 &operator-=(T n)                      { x -= n; y -= n; z -= n; return *this; }        ///< @private
    Vec3 &operator*=(T n)                      { x *= n; y *= n; z *= n; return *this; }        ///< @private
    Vec3 &operator/=(T n)                      { x /= n; y /= n; z /= n; return *this; }        ///< @private
    Vec3 &operator+=(const Vec3 &v)            { x += v.x; y += v.y; z += v.z; return *this; }  ///< @private
    Vec3 &operator-=(const Vec3 &v)            { x -= v.x; y -= v.y; z -= v.z; return *this; }  ///< @private
    Vec3 &operator*=(const Vec3 &v)            { x *= v.x; y *= v.y; z *= v.z; return *this; }  ///< @private
    Vec3 &operator/=(const Vec3 &v)            { x /= v.x; y /= v.y; z /= v.z; return *this; }  ///< @private
    constexpr bool operator==(const Vec3 &v)   { return (v.x == x && v.y == y && v.z == z); }   ///< @private
    constexpr bool operator!=(const Vec3 &v)   { return !(*this == v); }                        ///< @private
};

using Point3 = Vec3<float>;

} // namespace gm::gfx
