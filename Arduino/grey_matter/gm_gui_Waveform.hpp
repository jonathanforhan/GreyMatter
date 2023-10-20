#pragma once
#include "gm_gfx_LCD.hpp"
#define GML_STL 1
#include "gm_stl/gm_stl.hpp"

namespace gm::gui {

class Waveform
{
public:
    Waveform(const gfx::LCD &lcd, long update_time, const char *title, const char *x_unit, const char *y_unit, const char *unit_symbol, bool loglog = false)
          : lcd(lcd),
            title(title),
            x_unit(x_unit),
            y_unit(y_unit),
            unit_symbol(unit_symbol),
            update_time(update_time),
            loglog(loglog)
    {
        last_read = millis();
    }

    void draw(gfx::Size bounds) const;
    void redraw(gfx::Size bounds) const;
    void export_last(char *sensor) const;
    bool should_update(long ms);
    // will slide data if data.size() >= 100
    void append(float x);  

    gfx::Size padding = { 20, 50 };
    gfx::Size dimensions = { 100, 100 };
    const char *title;
    const char *x_unit;
    const char *y_unit;
    const char *unit_symbol;
    bool loglog;

    String current_read;
    String mean;
    String stdev;
    String local_max;
    String local_min;

private:
    std::vector<float> data;
    long update_time;
    long last_read;

    const gfx::LCD &lcd;
};

} // namespace gm::gui
