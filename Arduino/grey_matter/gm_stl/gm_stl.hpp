#pragma once

#if GML_STL
#   include "algorithm.hpp"
#   include "initializer_list.hpp"
#   include "iterator.hpp"
#   include "vector.hpp"
#endif // GML_STL

#define MAIN() void loop(){} void setup()
#define LOOP(_Expr) for (;;_Expr)
