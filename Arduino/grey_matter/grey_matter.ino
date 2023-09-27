#define GML_STL 1
#include "gml.hpp"

int main()
{
    Serial.begin(9600);

    try {
        for (;;)
        {
            auto key = gml::reserve_chunk(gml::ChunkType::eLarge);
            char *foo = gml::access_chunk(key);

            for (int i = 0; i < gml::get_chunk_size(key); i++)
                foo[i] = i % 26 + 'a';

            for (int i = 0; i < gml::get_chunk_size(key); i++)
                Serial.print(foo[i]);

            Serial.println("\nfree");
            gml::free_chunk(key);
        }
    } catch(...) {
        for (;;) Serial.println("MAIN PANIC");
    }

    return 0;
}
