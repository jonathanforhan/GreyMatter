#pragma once
#include "gm_gfx_LCD.hpp"
#define GML_STL 1
#include "gm_stl/gm_stl.hpp"

namespace gm::gui {

class Waveform
{
public:
    Waveform(const gfx::LCD &lcd, long update_time, const char *title = nullptr, char x_unit = '\0', char y_unit = '\0')
          : lcd(lcd),
            title(title),
            x_unit(x_unit),
            y_unit(y_unit),
            update_time(update_time)
    {
        last_read = millis();
    }

    void draw() const;
    void redraw() const;
    bool should_update(long ms);

    gfx::Size padding = { 20, 50 };
    gfx::Size dimensions = { 100, 100 };
    const char *title;
    char x_unit;
    char y_unit;
    std::vector<float> data;

private:
    long update_time;
    long last_read;

    const gfx::LCD &lcd;
};

} // namespace gm::gui
