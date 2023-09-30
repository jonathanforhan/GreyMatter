#define GML_STL 1
#include "gm_stl/gm_stl.hpp"
#include "gm_gfx_LCD.hpp"

using namespace gm;

MAIN()
{
    Serial.begin(9600);
    gfx::LCD lcd(A3, A2, A1, A0, A4);
    lcd.reset();
    lcd.begin();
    lcd.set_rotation(gfx::Rotation::r270);
    lcd.set_cursor({ 10, 10 });

    lcd.fill_screen(gfx::Black);
    lcd.print("Hello, World");
    lcd.fill_rect({ 20, 20 }, { 50, 50 }, gfx::Red);
    LOOP()
    {
    }
}
