#pragma once
#define GM_USE_9341 1
#include "gm_gfx_LCD.hpp"
#include "gm_gui_Cube.hpp"
#include "gm_gui_Waveform.hpp"

namespace gm::gui {

enum class CallbackAction
{
    Idle,
    Redraw,
    Draw,
};

// has LCD components, helps with header conflicts and abstraction
class GUI
{
public:
    GUI();
    void begin();

    template <class F>
    void loading_screen(const char *msg, F &&fn);

    void call(size_t i, CallbackAction action)
    {
        callbacks[i](millis(), action);
    }

    gfx::LCD lcd;
    std::vector<void(*)(long, CallbackAction)> callbacks;
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
        theta += 0.15f;
        cube.rotate_y(theta);
        cube.rotate_x(tilt);
        cube.scale({ 12, 12, 12 });
        cube.offset(lcd.dimensions() / 2);
        cube.draw(gfx::White);
        delay(25);

        if (fn()) break;
    }
    lcd.fill_screen(gfx::Black);
}

} // namespace gm::gui