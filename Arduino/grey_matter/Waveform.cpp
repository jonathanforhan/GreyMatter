#include "Waveform.hpp"

namespace gmlcd {

Waveform::Waveform(GMLCD &lcd)
      : _lcd(lcd)
{}

Waveform::draw(const std::vector<float> &data, bool shifted)
{
    if (data.size() < 2)
        return;

    _lcd.drawFastVLine({ hpadding + 1, vpadding }, height - 1, Color::Black);

    for (size_t i = 0; i < data.size(); i++)
    {
        Point px = { i + hpadding + 1, (height - data[i]) + vpadding };
        if (data[i] > 0 && data[i] < 101)
            _lcd.drawPixel(px, Color::White);

        if (i != data.size() - 1 && data[i] != data[i+1])
        {
            px.x++;
            _lcd.drawPixel(px, Color::Black);
        }
    }

    _lcd.setTextSize(1);
    auto new_vpadding = 2 * vpadding + height;
    Size readout_size = _lcd.getTextBounds("Current Humidity: ");
    Point readout_pos = { hpadding + readout_size.w, new_vpadding };
    readout_size = _lcd.getTextBounds("0000000%");
    _lcd.fillRect(readout_pos, readout_size, Color::Black);
    _lcd.setCursor(hpadding, new_vpadding);
    _lcd.print(String("Current Humidity: ") + data.back() + "%");
}

Waveform::redraw()
{
    _lcd.fillScreen(Color::Black);
    _lcd.drawFastVLine({ hpadding, vpadding }, height, Color::Green);
    _lcd.drawFastHLine({ hpadding, vpadding + height }, width, Color::Red);
}

} // namespace gmlcd
