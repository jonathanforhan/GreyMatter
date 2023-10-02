#include "gm_gui_Waveform.hpp"

namespace gm::gui {
using namespace gfx;

void Waveform::draw() const
{
    if (data.size() < 2)
        return;

    lcd.draw_v_line(padding + Size{ 1, 0 }, dimensions.y - 1, Black);

    for (size_t i = 0; i < data.size(); i++)
    {
        Pixel px = padding + Pixel{ i + 1, dimensions.y - data[i] };
        if (data[i] > 0 && data[i] < 101)
            lcd.draw_px(px, Color::White);

        if (i != data.size() - 1 && data[i] != data[i+1])
        {
            px.x++;
            lcd.draw_px(px, Color::Black);
        }
    }

    lcd.set_text_size(1);

    auto new_vpadding = 2 * padding.y + dimensions.y;

    auto msg = String("Current ") + title + ": ";
    Size readout_size = lcd.get_text_dimensions(msg);

    Pixel readout_pos = { padding.x + readout_size.x, new_vpadding };
    readout_size = lcd.get_text_dimensions("0000000%");
    lcd.fill_rect(readout_pos, readout_size, Color::Black);
    lcd.set_cursor(padding.x, new_vpadding);

    lcd.print(msg + data.back() + y_unit);
}

void Waveform::redraw() const
{
    lcd.fill_screen(gfx::Black);
    lcd.draw_v_line(padding, dimensions.y, Green);
    lcd.draw_h_line(padding + Size{ 0, dimensions.y }, dimensions.x, Red);
    if (title)
    {
        lcd.set_text_size(1);
        lcd.print_centered(title, 20);
    }
    draw();
}

bool Waveform::should_update(long ms)
{
    if (ms >= last_read + update_time)
    {
        last_read = ms;
        return true;
    }
    return false;
}

} // namespace gm::gui
