#include "gm_gui_GUI.hpp"
#include "gm_io_IRcodes.hpp"

namespace gm::gui {

int GUI_VALUE;

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

void GUI::update()
{
    for (size_t i = 0; i < callbacks.size(); i++)
    {
        i == index
            ? call(i, gm::gui::CallbackAction::Draw)
            : call(i, gm::gui::CallbackAction::Idle);
    }
}

void GUI::on_input(uint16_t code)
{
    // avoid spam clicks
    static long prev = millis();
    long now = millis();

    if (now - prev >= 200)
        prev = now;
    else
        return;

    switch (code)
    {
    case gm::io::IR_NEXT:
        if (calibrating)
        {
            call(index, gm::gui::CallbackAction::Next);
        }
        else
        {
            index = (index + 1) % callbacks.size();
            call(index, gm::gui::CallbackAction::Redraw);
        }
        break;
    case gm::io::IR_BACK:
        if (calibrating)
        {
            call(index, gm::gui::CallbackAction::Back);
        }
        else
        {
            index = (index - 1 + callbacks.size()) % callbacks.size();
            call(index, gm::gui::CallbackAction::Redraw);
        }
        break;
    case gm::io::IR_EQ:
        call(index, gm::gui::CallbackAction::Reset);
        break;
    case gm::io::IR_ST:
        for (int i = 0; i < callbacks.size(); i++)
            call(i, gm::gui::CallbackAction::ResetAll);
        break;
    case gm::io::IR_UP:
        call(index, gm::gui::CallbackAction::Up);
        break;
    case gm::io::IR_DOWN:
        call(index, gm::gui::CallbackAction::Down);
        break;
    case gm::io::IR_PLAY:
        call(index, gm::gui::CallbackAction::Calibrate);
        calibrating = true;
        break;
    case gm::io::IR_FUNC:
        call(index, gm::gui::CallbackAction::Redraw);
        calibrating = false;
        break;
    case gm::io::IR_VOLP:
        call(index, gm::gui::CallbackAction::Increment);
        break;
    case gm::io::IR_VOLM:
        call(index, gm::gui::CallbackAction::Decrement);
        break;
    case gm::io::IR_ZERO:
        GUI_VALUE = 0;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_ONE:
        GUI_VALUE = 1;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_TWO:
        GUI_VALUE = 2;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_THREE:
        GUI_VALUE = 3;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_FOUR:
        GUI_VALUE = 4;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_FIVE:
        GUI_VALUE = 5;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_SIX:
        GUI_VALUE = 6;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_SEVEN:
        GUI_VALUE = 7;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_EIGHT:
        GUI_VALUE = 8;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    case gm::io::IR_NINE:
        GUI_VALUE = 9;
        call(index, gm::gui::CallbackAction::ValueQuery);
        break;
    }
}

} // namespace gm::gui