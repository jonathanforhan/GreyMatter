#define GML_STL 1
#include "gml/gml.hpp"
#include "gml.test.hpp"

#define DEBUG true

int main()
{
    Serial.begin(9600);

#if DEBUG
    LOOP(gml::test_vector());
#endif // NDEBUG


    return 0;
}
