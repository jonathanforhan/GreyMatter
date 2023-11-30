#include "gm_gui_Calibration.hpp"
#include "gm_io_IRcodes.hpp"
#include "gm_gfx_Vec2.hpp"
#include <EEPROM.h>

#define EEPROM_STORE_OFFSET 128

namespace gm::gui {

Calibration::Calibration(gm::gfx::LCD &lcd, std::initializer_list<CalibrationInfo> &&values)
    : lcd(lcd), values(values)
{
    static int eIndex = 0;

    for (auto &value : this->values)
    {
        uint8_t *default_val = (uint8_t *)&value.default_value;
        EEPROM.write(eIndex * sizeof(float) + 0, default_val[0]);
        EEPROM.write(eIndex * sizeof(float) + 1, default_val[1]);
        EEPROM.write(eIndex * sizeof(float) + 2, default_val[2]);
        EEPROM.write(eIndex * sizeof(float) + 3, default_val[3]);
        value_positions.push_back(eIndex);
        eIndex++;
    }

    read_store_values();
}

void Calibration::draw()
{
    lcd.set_text_size(2);

    for (int i = 0; i < values.size(); i++)
    {
        lcd.set_cursor(20, 60 + i * 30);

        uint8_t prec = values[i].precision == 0 ? 2 : 4;
        String s = String(*values[i].value, prec);

        auto pos = lcd.cursor();
        pos.x += lcd.get_text_dimensions(values[i].title).x + 12;
        lcd.set_cursor(pos);

        for (int j = 0; j < s.length(); j++)
        {
            // reset
            if (i == last_position.x && j == last_position.y)
            {
                lcd.set_text_color(gm::gfx::White, gm::gfx::Black);
                lcd.print(s[j]);
            }
            // modify
            else if (i == position.x && j == position.y)
            {
                lcd.set_text_color(gm::gfx::Black, gm::gfx::White);
                lcd.print(s[j]);
            }
            else
            {
                lcd.set_cursor(lcd.cursor() + gm::gfx::Position{12, 0});
            }
        }
    }

    lcd.set_text_size(1);
}

void Calibration::redraw()
{
    lcd.fill_screen(gfx::Black);
    lcd.set_text_size(2);

    lcd.set_text_color(gm::gfx::White, gm::gfx::Black);
    lcd.print_centered("GM Calibration GUI", 20);

    for (int i = 0; i < values.size(); i++)
    {
        lcd.set_cursor(20, 60 + i * 30);
        lcd.print(values[i].title);
        lcd.print(' ');

        uint8_t prec = values[i].precision == 0 ? 2 : 4;
        String s = String(*values[i].value, prec);

        for (int j = 0; j < s.length(); j++)
        {
            i == position.x &&j == position.y
                ? lcd.set_text_color(gm::gfx::Black, gm::gfx::White)
                : lcd.set_text_color(gm::gfx::White, gm::gfx::Black);

            lcd.print(s[j]);
        }
    }

    lcd.set_text_size(1);
}

void Calibration::redraw_on_update()
{
    lcd.set_text_size(2);

    for (int i = 0; i < values.size(); i++)
    {
        lcd.set_cursor(20, 60 + i * 30);

        uint8_t prec = values[i].precision == 0 ? 2 : 4;
        String s = String(*values[i].value, prec);

        auto pos = lcd.cursor();
        pos.x += lcd.get_text_dimensions(values[i].title).x + 12;
        lcd.set_cursor(pos);

        if (i == position.x)
        {
            auto dim = lcd.get_text_dimensions(s + "...");
            lcd.fill_rect(lcd.cursor(), dim, gm::gfx::Black);
        }
    }

    uint8_t prec = values[position.x].precision == 0 ? 2 : 4;
    String s = String(*values[position.x].value, prec);
    if (s[position.y] == '.' || s[position.y] == '-')
        position.y++;

    for (int i = 0; i < values.size(); i++)
    {
        lcd.set_cursor(20, 60 + i * 30);

        prec = values[i].precision == 0 ? 2 : 4;
        s = String(*values[i].value, prec);

        auto pos = lcd.cursor();
        pos.x += lcd.get_text_dimensions(values[i].title).x + 12;
        lcd.set_cursor(pos);

        for (int j = 0; j < s.length(); j++)
        {
            i == position.x &&j == position.y
                ? lcd.set_text_color(gm::gfx::Black, gm::gfx::White)
                : lcd.set_text_color(gm::gfx::White, gm::gfx::Black);

            lcd.print(s[j]);
        }
    }

    lcd.set_text_size(1);
}

void Calibration::handle_ir_action(gm::gui::CallbackAction action)
{
    if (!is_active() && action == gm::gui::CallbackAction::ResetAll)
        return reset_all();
    else if (!is_active())
        return;

    switch (action)
    {
    case gm::gui::CallbackAction::Next:
        next_decimal_place();
        break;
    case gm::gui::CallbackAction::Back:
        prev_decimal_place();
        break;
    case gm::gui::CallbackAction::Up:
        next_value();
        break;
    case gm::gui::CallbackAction::Down:
        next_value();
        break;
    case gm::gui::CallbackAction::Increment:
        modify_value(1);
        break;
    case gm::gui::CallbackAction::Decrement:
        modify_value(-1);
        break;
    case gm::gui::CallbackAction::ValueQuery:
        set_value(GUI_VALUE);
        break;
    case gm::gui::CallbackAction::Reset:
        reset();
        break;
    case gm::gui::CallbackAction::ResetAll:
        reset_all();
        break;
    default:
        return;
    }
    draw();
}

void Calibration::reset()
{
    uint8_t val[4];
    val[0] = EEPROM.read(value_positions[position.x] * sizeof(float) + 0);
    val[1] = EEPROM.read(value_positions[position.x] * sizeof(float) + 1);
    val[2] = EEPROM.read(value_positions[position.x] * sizeof(float) + 2);
    val[3] = EEPROM.read(value_positions[position.x] * sizeof(float) + 3);

    *values[position.x].value = *(float *)val;

    store_values();
    redraw_on_update();
}

void Calibration::reset_all()
{
    int i = 0;
    for (auto value_pos : value_positions)
    {
        uint8_t val[4];
        val[0] = EEPROM.read(value_pos * sizeof(float) + 0);
        val[1] = EEPROM.read(value_pos * sizeof(float) + 1);
        val[2] = EEPROM.read(value_pos * sizeof(float) + 2);
        val[3] = EEPROM.read(value_pos * sizeof(float) + 3);

        *values[i].value = *(float *)val;
        i++;
    }

    store_values();
}

void Calibration::modify_value(int16_t v)
{
    uint8_t prec = values[position.x].precision == 0 ? 2 : 4;
    String s = String(*values[position.x].value, prec);

    int dot_location = s.indexOf('.');
    int multiplier = dot_location - position.y;
    if (position.y < dot_location)
        multiplier -= 1;

    float val = pow(10, multiplier);

    *values[position.x].value += (val * v);

    store_values();
    redraw_on_update();
}

void Calibration::set_value(int16_t v)
{
    uint8_t prec = values[position.x].precision == 0 ? 2 : 4;
    String s = String(*values[position.x].value, prec);

    int dot_location = s.indexOf('.');
    int multiplier = dot_location - position.y;
    if (position.y < dot_location)
        multiplier -= 1;

    float val = pow(10, multiplier);
    int diff = s[position.y] - '0';

    *values[position.x].value -= (val * diff);
    *values[position.x].value += (val * v);

    store_values();
    redraw_on_update();
}

void Calibration::store_values()
{
    int i = 0;
    for (auto value_pos : value_positions)
    {
        uint8_t *val = (uint8_t *)values[i].value;
        EEPROM.write(value_pos * sizeof(float) + 0 + EEPROM_STORE_OFFSET, val[0]);
        EEPROM.write(value_pos * sizeof(float) + 1 + EEPROM_STORE_OFFSET, val[1]);
        EEPROM.write(value_pos * sizeof(float) + 2 + EEPROM_STORE_OFFSET, val[2]);
        EEPROM.write(value_pos * sizeof(float) + 3 + EEPROM_STORE_OFFSET, val[3]);
        i++;
    }
}

void Calibration::read_store_values()
{
    int i = 0;
    for (auto value_pos : value_positions)
    {
        uint8_t val[4];
        val[0] = EEPROM.read(value_pos * sizeof(float) + 0 + EEPROM_STORE_OFFSET);
        val[1] = EEPROM.read(value_pos * sizeof(float) + 1 + EEPROM_STORE_OFFSET);
        val[2] = EEPROM.read(value_pos * sizeof(float) + 2 + EEPROM_STORE_OFFSET);
        val[3] = EEPROM.read(value_pos * sizeof(float) + 3 + EEPROM_STORE_OFFSET);
        *values[i].value = *(float *)val;
        i++;
    }
}

} // namespace gm::gui