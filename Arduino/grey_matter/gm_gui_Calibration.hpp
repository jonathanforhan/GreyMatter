#pragma once
#include "gm_gfx_LCD.hpp"
#define GML_STL 1
#include "gm_stl/gm_stl.hpp"

namespace gm::gui {

/// @brief Calibration Packet sent to calibrator
struct CalibrationInfo
{
    float &value;      ///< @brief value ref for mutating value
    const char *title; ///< @brief display title for value
};

/// @brief Calibration class, work in junction with Waveform to provide user calibration
class Calibration
{
public:
    /// @brief Construct a Calibration object
    /// @param lcd LCD ref
    /// @param values List of values required to modify
    /// @note recommended to create this as *static* if used in a gui callback
    Calibration(gm::gfx::LCD &lcd, std::vector<CalibrationInfo> &values) : lcd(lcd), values(values) {}

    /// @brief Draw the waveform to screen
    void draw();

    /// @brief Redraw the waveform this is much faster than draw but requires that it was drawn initially
    /// @param bounds bounds of waveform
    void redraw();

    /// @brief Modify a value in the values list
    /// @param index index of value in list
    /// @param position decimal place to modify 0 -> 10 | 1 -> 1 | 2 -> 0.1 | 3 -> 0.01
    void modifyValue(size_t index, size_t position);

    /// @brief Query active state
    /// @return yes/no
    bool isActive() const { return active; }

    /// @brief Set calibration active
    /// @param true/false
    void setActive(bool b) { active = b; }

private:
    gm::gfx::LCD& lcd;
    std::vector<CalibrationInfo> values;
    bool active = false;
};

} // namespace gm::gui