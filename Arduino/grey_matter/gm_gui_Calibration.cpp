#include "gm_gui_Calibration.hpp"

namespace gm::gui {
    
void Calibration::draw()
{
}

void Calibration::redraw()
{
    lcd.fill_screen(gfx::Black);
    lcd.set_cursor(20, 20);

    for (int i = 0; i < values.size(); i++)
    {
        lcd.print(values[i].title);
        lcd.print(' ');
        lcd.print(values[i].value);

        lcd.set_cursor(20, 20 + i * 20);
    }
}

void Calibration::modifyValue(size_t index, size_t position)
{
}

} // namespace gm::gui