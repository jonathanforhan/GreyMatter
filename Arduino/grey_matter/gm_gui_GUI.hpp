#pragma once

/// @file gm_gui_GUI.hpp
/// @brief high-level Graphical abstraction for customer to make a nice UI

/// @def GM_USE_9341 
/// @brief An optimization to bypass driver checks on this model
#define GM_USE_9341 1
#include "gm_gfx_LCD.hpp"
#include "gm_gui_Cube.hpp"
#include "gm_gui_Waveform.hpp"

namespace gm::gui {

/// @brief Queriable value representing any info
extern int GUI_VALUE;

/// @brief Action passed to a GUI callback it can respond accordinly
enum class CallbackAction
{
    Idle,
    Redraw,
    Draw,
    Calibrate,
    Next,
    Back,
    Up,
    Down,
    Increment,
    Decrement,
    ValueQuery,
    Reset,
    ResetAll,
};

/// @brief has LCD component, helps with header conflicts and abstraction
/// Higher level abstraction on LCD that contains callbacks to run
class GUI
{
public:
    /// @brief construct GUI
    /// @note implicitly non-copyable due to LCD
    GUI();

    /// @brief begin GUI, clears screen and sets rotation
    void begin();

    /// @brief Loading screen callback
    /// @tparam F C++ datatype of the callback
    /// @param msg message to print
    /// @param fn function to run on load
    template <class F>
    void loading_screen(const char *msg, F &&fn);

    /// @brief Call a calback in list with specified action
    /// @param i index of callback
    /// @param action action to pass
    void call(size_t i, CallbackAction action);

    /// @brief Upsate callbacks
    void update();

    /// @brief On external IR input
    /// @param code IRcode
    void on_input(uint16_t code);

    /// @brief LCD
    gfx::LCD lcd;

    /// @brief callbacks list
    std::vector<void(*)(long, CallbackAction)> callbacks;

    /// @brief Index of waveform
    int index = 0;

    /// @brief is the GUI calibrating
    bool calibrating = false;
};
    
template <class F>
void GUI::loading_screen(const char *msg, F &&fn)
{
    lcd.set_text_size(2);
    lcd.print_centered("Grey Matter", 60);
    lcd.set_text_size(1);
    lcd.print_centered(msg, 160);
    Cube cube(lcd);
    float theta = 0.0f;
    const float tilt = 0.25f;
    for (;;)
    {
        theta += 0.1f;
        cube.rotate_y(theta);
        cube.rotate_x(tilt);
        cube.scale({ 12, 12, 12 });
        cube.offset(lcd.dimensions() / 2);
        cube.draw(gfx::White);

        if (fn()) break;
    }
    call(index, gm::gui::CallbackAction::Redraw);
}

} // namespace gm::gui