#pragma once

#if GML_STL
#   include "algorithm.hpp"
#   include "initializer_list.hpp"
#   include "iterator.hpp"
#   include "vector.hpp"
#endif // GML_STL

#define gm_main() loop(){} void setup()
#define gm_loop(_Expr) for (;;_Expr)
