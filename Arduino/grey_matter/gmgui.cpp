#include "gmgui.hpp"

namespace gmlcd {

void GMGUI::init()
{
    _lcd.reset();
    _lcd.begin();
    _lcd.setRotation(3);
    _lcd.fillScreen(Color::Black);
}

} // namespace gmlcd