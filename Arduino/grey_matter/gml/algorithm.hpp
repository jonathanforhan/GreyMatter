#pragma once

#if GML_STL

#include "iterator.hpp"

#pragma GCC system_header

#undef min
#undef max

/* extremely trimmed version of stl algorithm */
namespace std {

template <class _Tp>
constexpr void swap(_Tp &__a, _Tp &__b)
{
    _Tp __tmp(__a);
    __a = __b;
    __b = __tmp;
}

template <class _Tp>
constexpr _Tp min(const _Tp &__a, const _Tp &__b)
{
    return __a < __b ? __a : __b;
}

template <class _Tp>
constexpr _Tp max(const _Tp &__a, const _Tp &__b)
{
    return __a > __b ? __a : __b;
}

template <class _Tp>
iterator<_Tp> find(iterator<_Tp> __first, iterator<_Tp> __last, const _Tp &__needle)
{
    for (; __first != __last; ++__first)
        if (*__first == __needle)
            break;
    return __first;
}

template <class _Tp>
iterator<_Tp> find(_Tp *__first, _Tp *__last, const _Tp &__needle)
{
    for (; __first != __last; ++__first)
        if (*__first == __needle)
            break;
    return __first;
}

} // namespace std

#endif