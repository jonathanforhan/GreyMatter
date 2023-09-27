#pragma once

#if GML_STL

#pragma GCC system_header

namespace std {

template <class _Tp>
class iterator
{
public:
    using iterator_type = const _Tp *;
    using size_type     = size_t;

    explicit constexpr iterator(_Tp *__ptr) noexcept : m_internal(__ptr) {}
    explicit constexpr iterator(const _Tp *__ptr) noexcept : m_internal(__ptr) {}

    iterator &operator++() noexcept                              { ++m_internal; return *this; }
    iterator &operator--() noexcept                              { --m_internal; return *this; }
    iterator &operator+=(size_type __n) noexcept                 { m_internal += __n; return *this; }
    iterator &operator-=(size_type __n) noexcept                 { m_internal -= __n; return *this; }

    constexpr iterator operator++(int) noexcept                  { auto __cpy = *this; ++m_internal; return __cpy; }
    constexpr iterator operator--(int) noexcept                  { auto __cpy = *this; --m_internal; return __cpy; }
    constexpr iterator operator+(size_type __n) const noexcept   { return { m_internal + __n }; }
    constexpr iterator operator-(size_type __n) const noexcept   { return { m_internal - __n }; }

    constexpr bool operator>(iterator<_Tp> __it) const noexcept  { return m_internal > __it.m_internal; }
    constexpr bool operator<(iterator<_Tp> __it) const noexcept  { return m_internal < __it.m_internal; }
    constexpr bool operator>=(iterator<_Tp> __it) const noexcept { return m_internal >= __it.m_internal; }
    constexpr bool operator<=(iterator<_Tp> __it) const noexcept { return m_internal <= __it.m_internal; }
    constexpr bool operator==(iterator<_Tp> __it) const noexcept { return m_internal == __it.m_internal; }
    constexpr bool operator!=(iterator<_Tp> __it) const noexcept { return !(m_internal == __it.m_internal); }

    _Tp &operator*() noexcept                                    { return *m_internal; }
    _Tp &operator*() const noexcept                              { return *m_internal; }
    _Tp *operator->() noexcept                                   { return m_internal; }
    _Tp *operator->() const noexcept                             { return m_internal; }

protected:
    iterator_type m_internal;
};

template <class _Tp>
class reverse_iterator
{
public:
    using reverse_iterator_type = const _Tp *;
    using size_type             = size_t;

    explicit constexpr reverse_iterator(_Tp *__ptr) noexcept : m_internal(__ptr) {}
    explicit constexpr reverse_iterator(const _Tp *__ptr) noexcept : m_internal(__ptr) {}

    reverse_iterator &operator++() noexcept                              { --m_internal; return *this; }
    reverse_iterator &operator--() noexcept                              { ++m_internal; return *this; }
    reverse_iterator &operator+=(size_type __n) noexcept                 { m_internal -= __n; return *this; }
    reverse_iterator &operator-=(size_type __n) noexcept                 { m_internal += __n; return *this; }

    constexpr reverse_iterator operator++(int) noexcept                  { auto __cpy = *this; --m_internal; return __cpy; }
    constexpr reverse_iterator operator--(int) noexcept                  { auto __cpy = *this; --m_internal; return __cpy; }
    constexpr reverse_iterator operator+(size_type __n) const noexcept   { return { m_internal - __n }; }
    constexpr reverse_iterator operator-(size_type __n) const noexcept   { return { m_internal + __n }; }

    constexpr bool operator>(reverse_iterator<_Tp> __it) const noexcept  { return m_internal > __it.m_internal; }
    constexpr bool operator<(reverse_iterator<_Tp> __it) const noexcept  { return m_internal < __it.m_internal; }
    constexpr bool operator>=(reverse_iterator<_Tp> __it) const noexcept { return m_internal >= __it.m_internal; }
    constexpr bool operator<=(reverse_iterator<_Tp> __it) const noexcept { return m_internal <= __it.m_internal; }
    constexpr bool operator==(reverse_iterator<_Tp> __it) const noexcept { return m_internal == __it.m_internal; }
    constexpr bool operator!=(reverse_iterator<_Tp> __it) const noexcept { return !(m_internal == __it.m_internal); }

    _Tp &operator*() noexcept                                            { return *m_internal; }
    _Tp &operator*() const noexcept                                      { return *m_internal; }
    _Tp *operator->() noexcept                                           { return m_internal; }
    _Tp *operator->() const noexcept                                     { return m_internal; }

protected:
    reverse_iterator_type m_internal;
};

} // namespace std

#endif // GML_S_TpL
