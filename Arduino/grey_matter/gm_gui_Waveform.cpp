#include "gm_gui_Waveform.hpp"

namespace gm::gui {
using namespace gfx;

void Waveform::draw(gfx::Size bounds)
{
    if (data.size() < 2)
        return;

    lcd.draw_v_line(padding + Size{ 1, 0 }, dimensions.y, Black);

    float min_val = 99999;
    float max_val = -99999;
    float numerator = 0;
    size_t n_elems = 0; // we ignore elements that fail bounding checks so we track n_elems independant of data.size()

    for (size_t i = 0; i < data.size(); i++)
    {
        float y = this->loglog ? log(data[i]) : data[i];
        if (!(y >= bounds.x && y < bounds.y))
            continue;

        y = (y / bounds.y) * 100;
        Pixel px = padding + Pixel{ i + 1, dimensions.y - y - 1 };
        lcd.draw_px(px, Color::White);

        min_val = std::min(min_val, data[i]);
        max_val = std::max(max_val, data[i]);
        numerator += data[i];
        n_elems++;

        if (i != data.size() - 1 && data[i] != data[i+1])
        {
            px.x++;
            lcd.draw_px(px, Black);
        }
    }

    Size pos = { lcd.width() / 2, padding.y };
    String msg;
    auto print_optimal = [this, &pos, &msg](String *base = nullptr) {
        for (size_t i = 0; i < msg.length(); i++, pos.x += 6)
            if (!base || i >= base->length() || (*base)[i] != msg[i])
                lcd.draw_char(pos.x, pos.y, msg[i], White, Black, 1);

        if (base)
        {
            for (size_t i = msg.length(); i < base->length(); i++, pos.x += 6)
                lcd.draw_char(pos.x, pos.y, ' ', White, Black, 1);
            *base = msg;
        }

        pos.x = lcd.width() / 2;
        pos.y += 12;
    };

    msg = String("Delta T: ") + update_time + "ms";
    print_optimal();
    msg = String("Current: ") + data.back() + " " + unit_symbol;
    print_optimal(const_cast<String *>(&current_read));
    msg = String("Mean: ") + numerator / n_elems + " " + unit_symbol;
    print_optimal(const_cast<String *>(&mean));
    // msg = String("Stdev: ") + data.back() + unit_symbol;
    // print_optimal(const_cast<String *>(&stdev));
    msg = String("Min: ") + min_val + " " + unit_symbol;
    print_optimal(const_cast<String *>(&local_min));
    msg = String("Max: ") + max_val + " " + unit_symbol;
    print_optimal(const_cast<String *>(&local_max));
}

void Waveform::redraw(gfx::Size bounds)
{
    lcd.fill_screen(gfx::Black);
    lcd.draw_v_line(padding, dimensions.y, Green);
    lcd.draw_h_line(padding + Size{ 0, dimensions.y }, dimensions.x, Red);
    if (title)
    {
        lcd.set_text_size(1);
        lcd.set_text_color(gm::gui::White, gm::gui::Black);
        lcd.print_centered(title, 20);
    }
    current_read = "";
    mean = "";
    stdev = "";
    local_max = "";
    local_min = "";
    draw(bounds);
}

void Waveform::export_last(char *sensor) const
{
    Serial.print(R"({"sensor":")");
    Serial.print(sensor);
    Serial.print(R"(","reading":")");
    Serial.print(this->data.back());
    Serial.println(R"("})");
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

void Waveform::append(float x)
{
    data.push_back(x);
    if (data.size() >= 100)
        data.pop_front();
}

} // namespace gm::gui
