#pragma once

#if GML_STL

#pragma GCC system_header

/* extremely trimmed version of stl algorithm */
namespace std {

template <class _Tp>
constexpr void swap(_Tp &__a, _Tp &__b)
{
    _Tp __tmp(__a);
    __a = __b;
    __b = __tmp;
}

} // namespace std

#endif