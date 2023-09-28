#define GML_STL 1
#include "gml/gml.hpp"
#include "gml.test.hpp"
#include "gmlcd.hpp"
#include "vec3.hpp"

#define DEBUG true

using namespace gmlcd;

void test();

GMLCD lcd;

void cache_line(const std::vector<Point> &vec, int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
        ystep = 1;
    else
        ystep = -1;

    for (; x0 <= x1; x0++)
    {
        if (steep)
            vec.push_back({ y0, x0 });
        else
            vec.push_back({ x0, y0 });
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

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

    static constexpr unsigned scale = 20;
    static constexpr unsigned offset = 120;
    static constexpr unsigned tilt = 25;
    float theta = 0;
    std::vector<Point> cache1;


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

        for (int i = 0; i < cache1.size(); i++)
            lcd.drawPixel(cache1[i], Color::Black);

        cache1.clear();
        cache_line(cache1, pts[0].x, pts[0].y, pts[1].x, pts[1].y);
        cache_line(cache1, pts[1].x, pts[1].y, pts[2].x, pts[2].y);
        cache_line(cache1, pts[2].x, pts[2].y, pts[3].x, pts[3].y);
        cache_line(cache1, pts[3].x, pts[3].y, pts[0].x, pts[0].y);
        cache_line(cache1, pts[4].x, pts[4].y, pts[5].x, pts[5].y);
        cache_line(cache1, pts[5].x, pts[5].y, pts[6].x, pts[6].y);
        cache_line(cache1, pts[6].x, pts[6].y, pts[7].x, pts[7].y);
        cache_line(cache1, pts[7].x, pts[7].y, pts[4].x, pts[4].y);
        for (int i = 0; i < 4; i++)
            cache_line(cache1, pts[i].x, pts[i].y, pts[i + 4].x, pts[i + 4].y);

        for (int i = 0; i < cache1.size(); i++)
            lcd.drawPixel(cache1[i], Color::White);
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
