#pragma once
#include <Arduino.h>

namespace gmlcd {
    
struct Size {
    uint16_t w;
    uint16_t h;

    Size operator+(uint16_t n)          { return { w + n, h + n }; }
    Size operator-(uint16_t n)          { return { w - n, h - n }; }
    Size operator*(uint16_t n)          { return { w * n, h * n }; }
    Size operator/(uint16_t n)          { return { w / n, h / n }; }
    Size &operator+=(uint16_t n)        { w += n; h += n; return *this; }
    Size &operator-=(uint16_t n)        { w -= n; h -= n; return *this; }
    Size &operator*=(uint16_t n)        { w *= n; h *= n; return *this; }
    Size &operator/=(uint16_t n)        { w /= n; h /= n; return *this; }
    Size operator+(const Size &size)    { return { w + size.w, h + size.h}; }
    Size operator-(const Size &size)    { return { w - size.w, h - size.h}; }
    Size operator*(const Size &size)    { return { w * size.w, h * size.h}; }
    Size operator/(const Size &size)    { return { w / size.w, h / size.h}; }
    Size &operator+=(const Size &size)  { w += size.w; h += size.h; return *this; }
    Size &operator-=(const Size &size)  { w -= size.w; h -= size.h; return *this; }
    Size &operator*=(const Size &size)  { w *= size.w; h *= size.h; return *this; }
    Size &operator/=(const Size &size)  { w /= size.w; h /= size.h; return *this; }
    constexpr bool operator==(const Size &size) { return (size.w == w && size.h == h); }
    constexpr bool operator!=(const Size &size) { return !(*this == size); }
};

} // namespace gmlcd
