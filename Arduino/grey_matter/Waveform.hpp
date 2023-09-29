#pragma once
#define GML_STL 1
#include "gml/gml.hpp"
#include "gmlcd.hpp"

namespace gmlcd {
    
class Waveform
{
public:
    Waveform(GMLCD &lcd);

    draw(const std::vector<float> &data, bool shifted = false);
    redraw();

    const uint16_t hpadding = 20;
    const uint16_t vpadding = 20;
    const uint16_t width = 100;
    const uint16_t height = 100;

private:
    GMLCD &_lcd;
};
    
} // namespace gmlcd
