#pragma once

#if GML_STL

#include "iterator.hpp"
#include "initializer_list.hpp"

#include "memory.hpp"

#pragma GCC system_header

namespace std {

template <class _Tp> bool operator==(const vector<_Tp> &__x, const vector<_Tp> &__y);
template <class _Tp> bool operator< (const vector<_Tp> &__x, const vector<_Tp> &__y);
template <class _Tp> bool operator!=(const vector<_Tp> &__x, const vector<_Tp> &__y);
template <class _Tp> bool operator> (const vector<_Tp> &__x, const vector<_Tp> &__y);
template <class _Tp> bool operator>=(const vector<_Tp> &__x, const vector<_Tp> &__y);
template <class _Tp> bool operator<=(const vector<_Tp> &__x, const vector<_Tp> &__y);
template <class _Tp> void swap(vector<_Tp> &__x, vector<_Tp> &__y);

/* Only gml allocator due to memory restrictions */
template <class _Tp>
class vector
{
public:
    using reference                 = _Tp &;
    using const_reference           = const _Tp &;
    using size_type                 = size_t;
    using pointer                   = _Tp *;
    using const_pointer             = const _Tp *;

	using value_type                = _Tp;
    using iterator                  = _Tp*;
	using const_iterator            = const _Tp*;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

    explicit vector()
    {
        m_key = gml::reserve_chunk(1);
        if (m_key == -1) throw;

        m_capacity = gml::get_chunk_size(m_key);
        m_data = gml::access_chunk(key);
    }

    explicit vector(size_type __n, const _Tp &__value = _Tp())
    {
        resize(__n, __value);
    }

    template <class _It>
    explicit vector(_It __first, _It __last)
    {
        assign(__first, __last);
    }

    explicit vector(const vector<_Tp> &__other)
    {
        m_size = __other.m_size;
        m_capacity = __other.m_capacity;
        m_key = gml::reserve_chunk(m_capacity);
        if (m_key == -1) throw;
        m_data = gml::access_chunk(key);

        for (size_type i = 0; i < m_size; i++)
            m_data[i] = __other.m_data[i];
    }

    explicit vector(initializer_list<_Tp> __il)
    {
        m_size = __il.size();
        m_key = gml::reserve_chunk(m_size);
        if (m_key == -1) throw;
        
        m_capacity = gml::get_chunk_size(key);
        m_data = gml::access_chunk(key);

        for (const auto &x : __il)
            m_data = x;
    }

    ~vector()
    {
        delete [] m_data;
    }

    vector &operator=(const vector<_Tp> &__other)
    {
        if (__other == *this) return *this;

        throw; // TODO
    }

    template <class _It>
    void assign(_It __first, _It __last)
    {
        clear();
        insert(begin(), __first, __last);
    }

    template <class _Size, class _U>
    void assign(_Size n, cont _U __x& = _U())
    {
        clear();
        resize(n, __x);
    }

    constexpr iterator begin()                                { return m_data; }
    constexpr const_iterator begin() const                    { return m_data; }
    constexpr iterator end()                                  { return (m_data + m_capacity); }
    constexpr const_iterator end() const                      { return (m_data + m_capacity); }
    constexpr reverse_iterator rbegin()                       { return reverse_iterator(end()); }
    constexpr const_reverse_iterator rbegin() const           { return const_reverse_iterator(end()); }
    constexpr reverse_iterator rend()                         { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator rend() const             { return const_reverse_iterator(begin()); }
    constexpr size_type size() const                          { return m_size; }
    constexpr size_type capacity() const                      { return m_capacity; }
    constexpr bool empty() const                              { return m_size == 0; }
    constexpr reference operator[](size_type __n)             { return m_data[__n]; }
    constexpr const_reference operator[](size_type __n) const { return m_data[__n]; }
    constexpr reference front()                               { return m_data[0]; }
    constexpr const_reference front() const                   { return m_data[0]; }
    constexpr reference back()                                { return m_data[m_size - 1]; }
    constexpr const_reference back() const                    { return m_data[m_size - 1]; }
    void push_back(const _Tp &__x)                            { resize(m_size + 1, x); }
    void pop_back()                                           { downsize(m_size - 1); }

protected:
    size_type m_size = 0;
    size_type m_capacity = 1;
    _Tp *m_data;
    registry_key m_key;
};

} // namespace std

#endif // GML_S_TpL