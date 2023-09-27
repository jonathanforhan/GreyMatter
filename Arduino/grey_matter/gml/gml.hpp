#pragma once
#include <Arduino.h>

#if GML_STL
#   include "algorithm.hpp"
#   include "initializer_list.hpp"
#   include "iterator.hpp"
#   include "vector.hpp"
#endif // GML_STL

#define LOOP(_Expr) do { _Expr; } while(1)
