#define GML_STL 1
#include "gml/gml.hpp"
#include "gmgui.hpp"
#include "DHT.h"

#define DHT22_PIN 50

using namespace gmlcd;

GMLCD lcd;
GMGUI gui(lcd);
Waveform waveform(lcd);

DHT dht22(DHT22_PIN, DHT22);

void setup()
{
    Serial.begin(9600);
    gui.init();
    dht22.begin();
}

void loop()
{
    float now = millis();
    std::vector<float> dht_data;

    gui.loadingScreen("initializing sensors...", [&]() {
        if (millis() >= now + 2000)
        {
            dht_data.push_back(dht22.readHumidity());
            now = millis();
        }
        return dht_data.size() >= 2;
    });

    waveform.redraw();
    waveform.draw(dht_data);
    now = millis();
    for (;;)
    {
        if (millis() >= now + 2000)
        {
            now = millis();
            dht_data.push_back(dht22.readHumidity());
            waveform.draw(dht_data);
            if (dht_data.size() == 96)
                dht_data.pop_front();
        }
    }
}
