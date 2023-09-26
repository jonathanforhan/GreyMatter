#pragma once

#if GML_STL

#include "iterator.hpp"

#pragma GCC system_header
#pragma GCC visibility push(default)
#pragma pack(push, 8)

namespace std {

template <class T>
class initializer_list
{
public:
    using value_type        = T;
    using refernce          = const T &;
    using const_refernce    = const T &;
    using size_type         = size_t;
    using iterator          = const T *;
    using const_iterator    = const T *;

private:
    // The compiler can call a private constructor.
    constexpr initializer_list(const_iterator __it, size_type __size)
          : m_array(__it),
            m_size(__size) {}

public:
    constexpr initializer_list() noexcept
          : m_array(0),
            m_size(0) {}

    constexpr size_type size() const noexcept
    { return m_size; }

    constexpr const_iterator begin() const noexcept
    { return m_array; }

    constexpr const_iterator end() const noexcept
    { return begin() + size(); }

private:
    iterator m_array;
    size_type m_size;

};

template <class T>
constexpr const T *begin(initializer_list<T> __il) noexcept
{ return __il.begin(); }

template <class T>
constexpr const T *end(initializer_list<T> __il) noexcept
{ return __il.end(); }

template <class T>
constexpr const std::reverse_iterator<T *> rbegin(initializer_list<T> __il) noexcept
{ return __il.end() - 1; }

template <class T>
constexpr const std::reverse_iterator<T *> rend(initializer_list<T> __il) noexcept
{ return __il.begin() - 1; }

} // namespace std

#pragma pack(pop)
#pragma GCC visibility pop

#endif // GML_STL
