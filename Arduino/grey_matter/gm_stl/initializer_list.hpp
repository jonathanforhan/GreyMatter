#pragma once

#if GML_STL

#include "iterator.hpp"

#pragma GCC system_header

namespace std {

template <class _Tp>
class initializer_list
{
public:
    using value_type        = _Tp;
    using refernce          = const _Tp &;
    using const_refernce    = const _Tp &;
    using size_type         = size_t;
    using iterator          = _Tp *;
    using const_iterator    = const _Tp *;

private:
    // _Tphe compiler can call a private constructor.
    constexpr initializer_list(const_iterator __it, size_type __size)
          : m_array(__it),
            m_size(__size) {}

public:
    constexpr initializer_list() noexcept
          : m_array(0),
            m_size(0) {}

    constexpr size_type size() const noexcept
    {
		return m_size;
	}

    constexpr const_iterator begin() const noexcept
    {
		return m_array;
	}

    constexpr const_iterator end() const noexcept
    {
		return m_array + m_size;
	}

private:
    iterator m_array;
    size_type m_size;

};

template <class _Tp>
constexpr const _Tp *begin(initializer_list<_Tp> __il) noexcept
{
    return __il.begin();
}

template <class _Tp>
constexpr const _Tp *end(initializer_list<_Tp> __il) noexcept
{
    return __il.end();
}

template <class _Tp>
constexpr const std::reverse_iterator<_Tp *> rbegin(initializer_list<_Tp> __il) noexcept
{
    return __il.end() - 1;
}

template <class _Tp>
constexpr const std::reverse_iterator<_Tp *> rend(initializer_list<_Tp> __il) noexcept
{
    return __il.begin() - 1;
}

} // namespace std

#endif // GML_S_TpL
