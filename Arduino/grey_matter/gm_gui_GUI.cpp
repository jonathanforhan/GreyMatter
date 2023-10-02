#include "gm_gui_GUI.hpp"

namespace gm::gui {

GUI::GUI() : lcd(gfx::LCD())
{
}

void GUI::begin()
{
    lcd.reset();
    lcd.begin();
    lcd.fill_screen(gfx::Black);
    lcd.set_rotation(gfx::r270);
}

void GUI::call(size_t i, CallbackAction action)
{
    callbacks[i](millis(), action);
}

} // namespace gm::gui