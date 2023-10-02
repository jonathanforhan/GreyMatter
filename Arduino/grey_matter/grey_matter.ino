#define GML_STL 1
#include "gm_stl/gm_stl.hpp"
#include "gm_gui_GUI.hpp"
#include <DHT.h>
#include <IRremote.hpp>

#define PREV 68
#define NEXT 67

#define IR_PIN      30
#define DHT22_PIN   50

gm::gui::GUI gui;
DHT dht22(DHT22_PIN, DHT22);

void dht22_humidity_callback(long, gm::gui::CallbackAction);
void dht22_temperature_callback(long, gm::gui::CallbackAction);

MAIN()
{
    Serial.begin(9600);
    gui.begin();
    dht22.begin();
    IrReceiver.begin(IR_PIN);

    gui.callbacks.push_back(dht22_humidity_callback);
    gui.callbacks.push_back(dht22_temperature_callback);

    auto now = millis();
    gui.loading_screen("initializing sensors...", [&]() -> bool {
        // one second delay per spec
        if (millis() >= now + 5000)
            return true;

        for (size_t i = 0; i < gui.callbacks.size(); i++)
            gui.call(i, gm::gui::CallbackAction::Idle);

        return false;
    });

    size_t index = 0;
    gui.call(index, gm::gui::CallbackAction::Redraw);
    LOOP()
    {
        for (size_t i = 0; i < gui.callbacks.size(); i++)
        {
            i == index
                ? gui.call(i, gm::gui::CallbackAction::Draw)
                : gui.call(i, gm::gui::CallbackAction::Idle);
        }

        if (IrReceiver.decode())
        {
            switch (IrReceiver.decodedIRData.command)
            {
            case PREV:
                index = (index - 1) % gui.callbacks.size();
                gui.call(index, gm::gui::CallbackAction::Redraw);
                break;
            case NEXT:
                index = (index + 1) % gui.callbacks.size();
                gui.call(index, gm::gui::CallbackAction::Redraw);
                break;
            }
            IrReceiver.resume();
        }
    }
}

void dht22_humidity_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 2000, "DHT22 Humidity", 's', '%');

    if (action == gm::gui::CallbackAction::Redraw)
        waveform.redraw();

    if (waveform.should_update(ms))
    {
        waveform.data.push_back(dht22.readHumidity());

        if (waveform.data.size() > 100)
            waveform.data.pop_front();

        if (action != gm::gui::CallbackAction::Idle)
            waveform.draw();
    }
}

void dht22_temperature_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 2000, "DHT22 Temperature", 's', 'C');

    if (action == gm::gui::CallbackAction::Redraw)
        waveform.redraw();

    if (waveform.should_update(ms))
    {
        waveform.data.push_back(dht22.readTemperature());

        if (waveform.data.size() > 100)
            waveform.data.pop_front();

        if (action != gm::gui::CallbackAction::Idle)
            waveform.draw();
    }
}
