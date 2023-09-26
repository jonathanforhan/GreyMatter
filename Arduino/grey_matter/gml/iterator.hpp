#pragma once

#if GML_STL

#pragma GCC system_header
#pragma GCC visibility push(default)

namespace std {

template <class T>
class iterator
{
public:
    using iterator_type = const T *;
    using size_type     = size_t;

    explicit constexpr iterator(T *__ptr) noexcept : m_internal(__ptr) {}
    explicit constexpr iterator(const T *__ptr) noexcept : m_internal(__ptr) {}

    iterator &operator++() noexcept                                 { ++m_internal; return *this; }
    iterator &operator--() noexcept                                 { --m_internal; return *this; }
    iterator &operator+=(size_type __n) noexcept                    { m_internal += __n; return *this; }
    iterator &operator-=(size_type __n) noexcept                    { m_internal -= __n; return *this; }
    constexpr iterator operator++(int) noexcept                     { auto __cpy = *this; ++m_internal; return __cpy; }
    constexpr iterator operator--(int) noexcept                     { auto __cpy = *this; --m_internal; return __cpy; }
    constexpr iterator operator+(size_type __n) const noexcept      { return { m_internal + __n }; }
    constexpr iterator operator-(size_type __n) const noexcept      { return { m_internal - __n }; }
    constexpr bool operator>(iterator<T> __it) const noexcept       { return m_internal > __it.m_internal; }
    constexpr bool operator<(iterator<T> __it) const noexcept       { return m_internal < __it.m_internal; }
    constexpr bool operator>=(iterator<T> __it) const noexcept      { return m_internal >= __it.m_internal; }
    constexpr bool operator<=(iterator<T> __it) const noexcept      { return m_internal <= __it.m_internal; }
    constexpr bool operator==(iterator<T> __it) const noexcept      { return m_internal == __it.m_internal; }
    constexpr bool operator!=(iterator<T> __it) const noexcept      { return !(m_internal == __it.m_internal); }
    T &operator*() noexcept                                         { return *m_internal; }
    T &operator*() const noexcept                                   { return *m_internal; }
    T *operator->() noexcept                                        { return m_internal; }
    T *operator->() const noexcept                                  { return m_internal; }

protected:
    iterator_type m_internal;
};

template <class T>
class reverse_iterator : iterator
{
public:
    constexpr reverse_iterator(T *__ptr) noexcept : iterator(__ptr) {}
    constexpr reverse_iterator(const T *__ptr) noexcept : iterator(__ptr) {}

    reverse_iterator &operator++() noexcept                         { --m_internal; return *this; }
    reverse_iterator &operator--() noexcept                         { ++m_internal; return *this; }
    reverse_iterator &operator+=(size_type __n) noexcept            { m_internal -= __n; return *this; }
    reverse_iterator &operator-=(size_type __n) noexcept            { m_internal += __n; return *this; }
    constexpr reverse_iterator operator++(int) noexcept             { auto __cpy = *this; --m_internal; return __cpy; }
    constexpr reverse_iterator operator--(int) noexcept             { auto __cpy = *this; ++m_internal; return __cpy; }
    constexpr reverse_iterator operator+(size_type __n) const noexcept { return { m_internal - __n }; }
    constexpr reverse_iterator operator-(size_type __n) const noexcept { return { m_internal + __n }; }
};

} // namespace std

#pragma GCC visibility pop

#endif // GML_STL
