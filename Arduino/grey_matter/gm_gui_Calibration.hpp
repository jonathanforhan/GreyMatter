#pragma once
#include "gm_gfx_LCD.hpp"
#define GML_STL 1
#include "gm_stl/gm_stl.hpp"
#include "gm_gfx_Vec2.hpp"
#include "gm_gui_GUI.hpp"

namespace gm::gui {

/// @brief Calibration Packet sent to calibrator
struct CalibrationInfo
{
    const char *title; ///< @brief display title for value
    float *value;      ///< @brief value ref for mutating value
    uint8_t precision; ///< @brief decimal place precision of value 2 by default
};

/// @brief Calibration class, work in junction with Waveform to provide user calibration
class Calibration
{
public:
    /// @brief Construct a Calibration object
    /// @param lcd LCD ref
    /// @param values List of values required to modify
    /// @note recommended to create this as *static* if used in a gui callback
    Calibration(gm::gfx::LCD &lcd, std::initializer_list<CalibrationInfo> &&values) : lcd(lcd), values(values) {}

    /// @brief Draw the waveform to screen
    void draw();

    /// @brief Redraw the waveform this is much faster than draw but requires that it was drawn initially
    void redraw();

    /// @brief limited redraw to fix any scaling issues with decimal places
    void redraw_on_update();

    /// @brief Helper to more ergonomically handle IR actions
    /// @param action action to handle
    void handle_ir_action(gm::gui::CallbackAction action);

    /// @brief modify value at this->position
    /// @param v the amount to modify
    void modify_value(int16_t v);

    /// @brief modify value at this->position
    /// @param v the new value
    void set_value(int16_t v);

    /// @brief Query active state
    /// @return yes/no
    bool is_active() const { return active; }

    /// @brief Set calibration active
    /// @param b true/false
    void set_active(bool b) { active = b; }

protected:
    /// @brief modify position
    void next_decimal_place()
    {
        last_position = position;

        position.y++;
        uint8_t prec = values[position.x].precision == 0 ? 2 : 4;
        String s = String(*values[position.x].value, prec);

        if (position.y < 0)
            position.y = s.length() - 1;
        else if (position.y >= s.length())
            position.y = 0;

        if (s[position.y] == '.' || s[position.y] == '-')
            position.y++;
    }

    /// @brief modify position
    void prev_decimal_place()
    {
        last_position = position;

        position.y--;
        uint8_t prec = values[position.x].precision == 0 ? 2 : 4;
        String s = String(*values[position.x].value, prec);

        if (position.y < 0)
            position.y = s.length() - 1;
        else if (position.y >= s.length())
            position.y = 0;

        if (s[position.y] == '.')
            position.y--;
        else if (s[position.y] == '-')
            position.y = s.length() - 1;
    }

    /// @brief next value in position list
    void next_value()
    {
        last_position = position;

        position.x++;
        if (position.x >= values.size())
            position.x = 0;

        position.y = 0;
    }

    /// @brief previous value in position list
    void prev_value()
    {
        last_position = position;

        position.x--;
        if (position.x < 0)
            position.x = values.size() - 1;

        position.y = 0;
    }

private:
    gm::gfx::LCD& lcd;
    std::vector<CalibrationInfo> values;
    bool active = false;
    gm::gfx::Position position{0, 0};
    gm::gfx::Position last_position{0, 0};
};

} // namespace gm::gui