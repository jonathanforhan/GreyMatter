/// @file grey_matter.ino
/// @brief main entry into Grey Matter Embedded
///
/// @warning A WARNING TO THE CUSTOMER the Arduino Compiler DOES NOT enforce const correctness
/// However Grey Matter API DOES respect const correctness so any methods called you will not have to worry about mutating const state
/// Documentation can be found at `docs/GM_Docs.html` Powered by Doxygen

#define GML_STL 1
#include "gm_stl/gm_stl.hpp"
#include "gm_gui_GUI.hpp"
#include "gm_gui_Calibration.hpp"
#include <DHT.h>
#include <IRremote.hpp>

#define IR_PIN      22
#define DHT22_PIN   50
#define WATER_PIN   A13
#define PHOTO_PIN   A14
#define UV_PIN      A15

gm::gui::GUI gui;
DHT dht22(DHT22_PIN, DHT22);

void dht22_humidity_callback(long, gm::gui::CallbackAction);
void dht22_temperature_callback(long, gm::gui::CallbackAction);
void photo_callback(long, gm::gui::CallbackAction);
void uv_callback(long, gm::gui::CallbackAction);
void water_callback(long, gm::gui::CallbackAction);

void gm_main()
{
    Serial.begin(9600);
    gui.begin();
    dht22.begin();
    IrReceiver.begin(IR_PIN);

    gui.callbacks.push_back(dht22_humidity_callback);
    gui.callbacks.push_back(dht22_temperature_callback);
    gui.callbacks.push_back(photo_callback);
    gui.callbacks.push_back(uv_callback);
    gui.callbacks.push_back(water_callback);

    auto now = millis();
    gui.loading_screen("initializing sensors...", [&]() -> bool {
        // one second delay per spec
        if (millis() >= now + 5000)
            return true;

        for (size_t i = 0; i < gui.callbacks.size(); i++)
            gui.call(i, gm::gui::CallbackAction::Idle);

        return false;
    });

    gm_loop()
    {
        gui.update();

        if (IrReceiver.decode())
        {
            gui.on_input(IrReceiver.decodedIRData.command);
            IrReceiver.resume();
        }
    }
}

void dht22_humidity_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 2000, "DHT22 Humidity", "Seconds", "Relative Humidity", "%");

    if (action == gm::gui::CallbackAction::Redraw)
        waveform.redraw({ 0, 100 });

    if (waveform.should_update(ms))
    {
        waveform.append(dht22.readHumidity());
        waveform.export_last("humidity");

        if (action != gm::gui::CallbackAction::Idle)
        {
            waveform.draw({0, 100});
        }
    }
}

void dht22_temperature_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 2000, "DHT22 Temperature", "Seconds", "Fahrenheit", "F");

    if (action == gm::gui::CallbackAction::Redraw)
        waveform.redraw({ 0, 100 });

    if (waveform.should_update(ms))
    {
        waveform.append(dht22.readTemperature(true));
        waveform.export_last("temperature");

        if (action != gm::gui::CallbackAction::Idle)
        {
            waveform.draw({ 0, 100 });
        }
    }
}

void photo_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 1000, "Photo Sensor", "Seconds", "Lux", "Lux", true);

    static float coef = 28.71;
    static float exp = 0.0075;

    static gm::gui::Calibration calibration(gui.lcd, {
        {
            .title = "Coefficient",
            .value = &coef,
        },
        {
            .title = "Exponent",
            .value = &exp,
            .precision = 4,
        }
    });

    auto calc_lux = [](float x) -> float { return coef * pow(M_E, (exp * x)); };

    switch (action)
    {
        case gm::gui::CallbackAction::Redraw:
            calibration.set_active(false);
            waveform.redraw({0, 12});
            break;
        case gm::gui::CallbackAction::Calibrate:
            calibration.set_active(true);
            calibration.redraw();
            break;
        default:
            if (calibration.is_active())
                calibration.handle_ir_action(action);
            break;
    }

    if (waveform.should_update(ms))
    {
        waveform.append(calc_lux(static_cast<float>(analogRead(PHOTO_PIN))));
        waveform.export_last("photo");

        if (action != gm::gui::CallbackAction::Idle)
        {
            if (!calibration.is_active()) {
                waveform.draw({0, 12});
            }
        }
    }
}

void uv_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 1000, "UV Sensor", "Seconds", "", "");

    auto calc_percent = [](float x) -> float { return (x / 1023.0f) * 100; };

    if (action == gm::gui::CallbackAction::Redraw)
        waveform.redraw({ 0, 100 });

    if (waveform.should_update(ms))
    {
        waveform.append(calc_percent(static_cast<float>(analogRead(UV_PIN))));
        waveform.export_last("uv");

        if (action != gm::gui::CallbackAction::Idle)
        {
            waveform.draw({ 0, 100 });
        }
    }
}

void water_callback(long ms, gm::gui::CallbackAction action)
{
    static gm::gui::Waveform waveform(gui.lcd, 1000, "Water Sensor", "Seconds", "", "");

    auto calc_percent = [](float x) -> float { return (x / 1023.0f) * 100; };

    if (action == gm::gui::CallbackAction::Redraw)
        waveform.redraw({ 0, 100 });

    if (waveform.should_update(ms))
    {
        waveform.append(calc_percent(static_cast<float>(analogRead(WATER_PIN))));
        waveform.export_last("water");

        if (action != gm::gui::CallbackAction::Idle)
        {
            waveform.draw({ 0, 100 });
        }
    }
}