#pragma once
#include "gmlcd.hpp"
#include "Cube.hpp"
#include "Waveform.hpp"

namespace gmlcd {

/* Components of GMGUI might rely on each other's header's so we move to another class */
class GMGUI
{
public:
    explicit GMGUI(GMLCD &lcd) : _lcd(lcd) {}

    void init();

    template <class F>
    void loadingScreen(const char *msg, F &&fn)
    {
        _lcd.setTextSize(2);
        _lcd.printCentered("Grey Matter", 60);
        _lcd.setTextSize(1);
        _lcd.printCentered(msg, 160);
        Cube cube(_lcd);
        float theta = 0.0f;
        float tilt = 0.25f;
        for (;;)
        {
            theta += 0.2f;
            cube.rotate_y(theta);
            cube.rotate_x(tilt);
            cube.scale(12);
            cube.offset({ LCD_WIDTH / 2, LCD_HEIGHT / 2 });
            cube.draw(Color::White);
            delay(100);

            if (fn()) break;
        }
        _lcd.fillScreen(Color::Black);
    }

private:
    GMLCD &_lcd;
};

} // namespace gmlcd