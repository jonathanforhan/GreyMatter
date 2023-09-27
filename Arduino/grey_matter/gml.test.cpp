#include "gml.test.hpp"

namespace gml {

bool test_vector()
{
    std::vector<int> vec = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };
    vec.insert(vec.begin() + 2, { 69, 420 });
    vec.push_front(100);

    for (const auto &e : vec)
    {
        Serial.println(e);
    }

    return true;
}

} // namespace gml
