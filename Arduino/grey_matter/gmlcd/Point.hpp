#pragma once
#include <Arduino.h>

namespace gmlcd {
    
struct Point {
    uint16_t x;
    uint16_t y;

    Point operator+(uint16_t n)           { return { x + n, y + n }; }
    Point operator-(uint16_t n)           { return { x - n, y - n }; }
    Point operator*(uint16_t n)           { return { x * n, y * n }; }
    Point operator/(uint16_t n)           { return { x / n, y / n }; }
    Point operator+(const Point &point)   { return { x + point.x, y + point.y}; }
    Point operator-(const Point &point)   { return { x - point.x, y - point.y}; }
    Point operator*(const Point &point)   { return { x * point.x, y * point.y}; }
    Point operator/(const Point &point)   { return { x / point.x, y / point.y}; }
    Point &operator+=(uint16_t n)         { x += n; y += n; return *this; }
    Point &operator-=(uint16_t n)         { x -= n; y -= n; return *this; }
    Point &operator*=(uint16_t n)         { x *= n; y *= n; return *this; }
    Point &operator/=(uint16_t n)         { x /= n; y /= n; return *this; }
    Point &operator+=(const Point &point) { x += point.x; y += point.y; return *this; }
    Point &operator-=(const Point &point) { x -= point.x; y -= point.y; return *this; }
    Point &operator*=(const Point &point) { x *= point.x; y *= point.y; return *this; }
    Point &operator/=(const Point &point) { x /= point.x; y /= point.y; return *this; }
    constexpr bool operator==(const Point &point) { return (point.x == x && point.y == y); }
    constexpr bool operator!=(const Point &point) { return !(*this == point); }
};

} // namespace gmlcd
