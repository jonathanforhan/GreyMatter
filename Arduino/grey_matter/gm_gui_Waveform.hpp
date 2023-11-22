#pragma once
#include "gm_gfx_LCD.hpp"
#define GML_STL 1
#include "gm_stl/gm_stl.hpp"

namespace gm::gui {

/// @brief Wavefor graph implementation
class Waveform
{
public:
    /// @brief Constuct a new waveform
    /// @param lcd LCD ref
    /// @param update_time time in ms of update
    /// @param title Graph title
    /// @param x_unit units for x axis
    /// @param y_unit units for y axis
    /// @param unit_symbol unit symbol
    /// @param loglog should display loglog?
    /// @warning in a callback passed to GUI it is recommended to create this class as *static* so it call live in the binary memory of the executable
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

    /// @brief Draw the waveform to screen
    /// @param bounds bounds of waveform
    void draw(gfx::Size bounds) const;

    /// @brief Redraw the waveform this is much faster than draw but requires that it was drawn initially
    /// @param bounds bounds of waveform
    void redraw(gfx::Size bounds) const;

    /// @brief Export the last waveform value as JSON over Serial
    /// @param sensor name of sensor in JSON
    void export_last(char *sensor) const;

    /// @brief Query whether the chart should update
    /// @param ms millis
    /// @return yes/no
    bool should_update(long ms);

    /// @brief Append a new data point
    /// @param x data point
    /// @note will slide data if data.size() >= 100
    void append(float x);

    gfx::Size padding = {20, 50};      ///< @brief chart padding
    gfx::Size dimensions = {100, 100}; ///< @brief chart dimensions
    const char *title;                 ///< @brief chart title
    const char *x_unit;                ///< @brief unit along x axis
    const char *y_unit;                ///< @brief unit along y axis
    const char *unit_symbol;           ///< @brief symbol of unit
    bool loglog;                       ///< @brief should print in loglog?

    String current_read; ///< @brief current value in waveform
    String mean;         ///< @brief average value across sliding window
    String stdev;        ///< @brief standard deviation of dataset
    String local_max;    ///< @brief local minimum
    String local_min;    ///< @brief local maximum

private:
    std::vector<float> data;
    long update_time;
    long last_read;

    const gfx::LCD &lcd;
};

} // namespace gm::gui
