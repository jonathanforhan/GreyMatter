#define GML_STL 1
#include "gml/gml.hpp"
#include "gml.test.hpp"
#include "gmlcd.hpp"
#include "vec3.hpp"

#define DEBUG true

using namespace gmlcd;

void test();

GMLCD lcd;

struct Cube {
    const vec3 raw[8] = {
            { -1, -1, -1 },
            { 1, -1, -1 },
            { 1, 1, -1 },
            { -1, 1, -1 },
            { -1, -1, 1 },
            { 1, -1, 1 },
            { 1, 1, 1 },
            { -1, 1, 1 },
    };
    vec3 pts[8] = {};
    vec3 cache[8] = {};

    unsigned scale = 15;
    unsigned offset = 120;
    unsigned tilt = 25;
    float theta = 0;


    void rotate()
    {
        for (int i = 0; i < 8; i++)
        {
            auto x_cpy = pts[i].x;
            auto y_cpy = pts[i].y;
            auto z_cpy = pts[i].z;
            pts[i].x = x_cpy * cos(theta) - z_cpy * sin(theta);
            pts[i].z = x_cpy * sin(theta) + z_cpy * cos(theta);

            y_cpy = pts[i].y;
            z_cpy = pts[i].z;
            pts[i].z = z_cpy * cos(tilt) - y_cpy * sin(tilt);
            pts[i].y = z_cpy * sin(tilt) + y_cpy * cos(tilt);
        }
        theta += 0.1;
    }

    void draw(const GMLCD &lcd)
    {
        for (int i = 0; i < 8; i++) for (int j = 0; j < 3; j++) pts[i][j] = raw[i][j];

        rotate();

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 3; j++)
                pts[i][j] = pts[i][j] * scale + offset;

        Point px = { offset - 2 * scale, offset - 2 * scale };
        lcd.fillRect(px, { scale * 4, scale * 4 }, Color::Black);

        for (int i = 0; i < 4; i++)
        {
            int j = i + 1;
            if (j % 4 == 0) j -= 4;
            lcd.drawLine({ pts[i].x, pts[i].y }, { pts[j].x, pts[j].y }, Color::White);
            lcd.drawLine({ pts[i + 4].x, pts[i + 4].y }, { pts[j + 4].x, pts[j + 4].y }, Color::White);
            lcd.drawLine({ pts[i].x, pts[i].y }, { pts[i + 4].x, pts[i + 4].y }, Color::White);
        }
    }
};
    

Cube cube;

void setup()
{
    Serial.begin(9600);
    test();
    lcd.reset();
    lcd.begin();
    lcd.setRotation(0);
    lcd.fillScreen(Color::Black);
    lcd.setCursor(10, 10);
    lcd.print("Grey Matter");
}

void loop()
{
    cube.draw(lcd);
    delay(100);
}

void test()
{
#if DEBUG
    if (!gml::test_vector()) for(;;){Serial.println("Vector Test Failed");}
    Serial.println("All Tests Passed");
#endif // DEBUG
}
