#pragma once

namespace gmlcd {
    
struct vec3
{
    float x, y, z;
    vec3() = default;

    float &operator[](int i)
    {
        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return z;
        }
    }
};
    
} // namespace gmlcd
