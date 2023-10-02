#pragma once

namespace gm::gfx {

template <class T>
struct Vec3
{
    T x, y, z;

    Vec3 operator+(T n)                        { return { x + n, y + n, z + n }; }
    Vec3 operator-(T n)                        { return { x - n, y - n, z - n }; }
    Vec3 operator*(T n)                        { return { x * n, y * n, z * n }; }
    Vec3 operator/(T n)                        { return { x / n, y / n, z / n }; }
    Vec3 operator+(const Vec3 &v)              { return { x + v.x, y + v.y, z + v.z }; }
    Vec3 operator-(const Vec3 &v)              { return { x - v.x, y - v.y, z - v.z }; }
    Vec3 operator*(const Vec3 &v)              { return { x * v.x, y * v.y, z * v.z }; }
    Vec3 operator/(const Vec3 &v)              { return { x / v.x, y / v.y, z / v.z }; }
    Vec3 &operator+=(T n)                      { x += n; y += n; z += n; return *this; }
    Vec3 &operator-=(T n)                      { x -= n; y -= n; z -= n; return *this; }
    Vec3 &operator*=(T n)                      { x *= n; y *= n; z *= n; return *this; }
    Vec3 &operator/=(T n)                      { x /= n; y /= n; z /= n; return *this; }
    Vec3 &operator+=(const Vec3 &v)            { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3 &operator-=(const Vec3 &v)            { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vec3 &operator*=(const Vec3 &v)            { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vec3 &operator/=(const Vec3 &v)            { x /= v.x; y /= v.y; z /= v.z; return *this; }
    constexpr bool operator==(const Vec3 &v)   { return (v.x == x && v.y == y && v.z == z); }
    constexpr bool operator!=(const Vec3 &v)   { return !(*this == v); }
};

using Point3 = Vec3<float>;

} // namespace gm::gfx
