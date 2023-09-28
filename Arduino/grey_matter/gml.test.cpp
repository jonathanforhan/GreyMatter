#include "gml.test.hpp"

namespace gml {

bool test_vector()
{
    std::vector<int> vec = {
            1, 2, 3
    };
    vec.insert(vec.begin() + 2, { 69, 420 });
    vec.push_front(100);
    vec.emplace_back(42);
    vec.erase(vec.begin() + 1, vec.begin() + 2);
    vec.shrink_to_fit();

    for (auto &elem : vec)
        ++elem;

    if (vec.size() != 6) return false;
    if (vec.capacity() != 6) return false;
    if (vec[0] != 101) return false;
    if (vec[1] != 3) return false;
    if (vec[2] != 70) return false;
    if (vec[3] != 421) return false;
    if (vec[4] != 4) return false;
    if (vec[5] != 43) return false;

    return true;
}

} // namespace gml
