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

} // namespace gm::gui