#pragma once

#if GML_STL

#include "algorithm.hpp"
#include "initializer_list.hpp"
#include "iterator.hpp"

#pragma GCC system_header

namespace std {

/// @brief Subset of std::vector for Arduino
/// @private
/// follows https://en.cppreference.com/w/cpp/container/vector
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

    explicit vector() noexcept
          : m_size(0),
            m_capacity(0),
            m_data(nullptr)
    {}

    explicit vector(size_type __n, const _Tp &__value = _Tp())
          : m_size(0),
            m_capacity(0),
            m_data(nullptr)
    {
        resize(__n, __value);
    }

    template <class _It>
    explicit vector(_It __first, _It __last)
          : m_size(0),
            m_capacity(0),
            m_data(nullptr)
    {
        assign(__first, __last);
    }

    explicit vector(const vector<_Tp> &__other)
          : m_size(0),
            m_capacity(0),
            m_data(nullptr)
    {
        assign(__other.begin(), __other.end());
    }

    vector(initializer_list<value_type> __il)
          : m_size(0),
            m_capacity(0),
            m_data(nullptr)
    {
        assign(__il);
    }

    ~vector()
    {
        delete [] m_data;
    }

    vector &operator=(const vector<value_type> &__other)
    {
        if (this == &__other) return;
        assign(__other.begin(), __other.end());
        return *this;
    }

    void assign(size_type __size, const_reference __x) noexcept
    {
        clear();
        insert(begin(), __size, __x);
    }

    template <class _It>
    void assign(_It __first, _It __last) noexcept
    {
        clear();
        insert(begin(), __first, __last);
    }

    void assign(initializer_list<_Tp> __il) noexcept
    {
        assign(__il.begin(), __il.end());
    }

    reference at(size_type __i)
    {
        return __i < m_size ? m_data[__i] : throw;
    }

    constexpr const_reference at(size_type __i) const
    {
		return __i < m_size ? m_data[__i] : throw;
	}

    reference operator[](size_type __i) noexcept
    {
		return m_data[__i];
	}

    constexpr const_reference operator[](size_type __i) const noexcept
    {
		return m_data[__i];
	}

    reference front() noexcept
    {
		return m_data[0];
	}

    constexpr const_reference front() const noexcept
    {
		return m_data[0];
	}

    reference back() noexcept
    {
		return m_data[m_size - 1];
	}

    constexpr const_reference back() const noexcept
    {
		return m_data[m_size - 1];
	}

    pointer data() noexcept
    {
		return m_data;
	}

    constexpr const_pointer data() const noexcept
    {
		return m_data;
	}

    iterator begin() noexcept
    {
		return { m_data };
	}

    const_iterator begin() const noexcept
    {
		return { m_data };
	}

    iterator end() noexcept
    {
		return { m_data + m_size };
	}

    const_iterator end() const noexcept
    {
		return { m_data + m_size };
	}

    reverse_iterator rbegin() noexcept
    {
		return { m_data + m_size - 1 };
	}

    const_reverse_iterator rbegin() const noexcept
    {
		return { m_data + m_size - 1 };
	}

    reverse_iterator rend() noexcept
    {
		return { m_data - 1 };
	}

    const_reverse_iterator rend() const noexcept
    {
		return { m_data - 1 };
	}

    constexpr bool empty() const noexcept
    {
		return m_size == 0;
	}

    constexpr size_type size() const noexcept
    {
		return m_size;
	}

    // max_size not implemented

    void reserve(size_type __n)
    {
        if (m_capacity >= __n) return;
        if (!m_capacity) m_capacity = 1;
        // while (m_capacity < __n) m_capacity <<= 1;
        if (m_capacity < __n) m_capacity = __n; // An Arduino optimization for constrained memory
        pointer __new_data = new value_type[m_capacity];

        for (size_type i = 0; i < m_size; i++)
            __new_data[i] = m_data[i];

        delete [] m_data;
        m_data = __new_data;
    }

    constexpr size_type capacity() const noexcept
    {
		return m_capacity;
	}

    void shrink_to_fit()
    {
        m_capacity = m_size;
        pointer __new_data = new value_type[m_capacity];

        for (auto i = 0; i < m_size; i++)
            __new_data[i] = m_data[i];

        delete [] m_data;
        m_data = __new_data;
    }

    void clear() noexcept
    {
		m_size = 0;
	}

    iterator insert(iterator __pos, const_reference __x)
    {
        const auto __index = __pos - m_data;
        reserve(m_size + 1);

        for (auto i = m_size; i > __index; i--)
            m_data[i] = m_data[i - 1];

        m_data[__index] = __x;
        m_size++;
        return m_data + __index;
    }

    iterator insert(iterator __pos, _Tp &&__x)
    {
        const auto __index = __pos - m_data;
        reserve(m_size + 1);

        for (auto i = m_size; i > __index; i--)
            m_data[i] = m_data[i - 1];

        m_data[__index] = __x;
        m_size++;
        return m_data + __index;
    }

    iterator insert(iterator __pos, size_type __n, const_reference __x)
    {
        const auto __index = __pos - m_data;
        reserve(m_size + __n);

        for (auto i = m_size + __n; i > (__index + __n - 1); i--)
            m_data[i] = m_data[i - __n];

        for (auto i = 0; i < __n; i++)
            m_data[i + __index] = __x;

        m_size += __n;
        return m_data + __index;
    }

    template <class _It>
    iterator insert(iterator __pos, _It __first, _It __last)
    {
        value_type __tmp;
        while (__first != __last)
        {
            __tmp = *__first;
            __pos = insert(__pos, __tmp);
            ++__first;
            ++__pos;
        }
        const auto __index = __pos - m_data;
        return m_data + __index;
    }

    iterator insert(iterator __pos, initializer_list<_Tp> __il)
    {
        return insert(__pos, __il.begin(), __il.end());
    }

    template <class ..._Args>
    iterator emplace(const_iterator __pos, _Args&&... __args)
    {
        const auto __index = __pos - m_data;
        m_size++;
        reserve(m_size);

        for (auto i = m_size - 1; i > __index; i--)
            m_data[i] = m_data[i - 1];

        m_data[__index] = value_type(__args...);
        return m_data + __index;
    }

    iterator erase(const_iterator __pos)
    {
        size_type __index = __pos - m_data;
        for (auto i = __index; i < (m_size - 1); i++)
            m_data[i] = m_data[i + 1];

        m_size--;
        return m_data + __index;
    }

    iterator erase(const_iterator __first, const_iterator __last)
    {
        size_type __index = __first - m_data;
        size_type __n = __last - __first;
        for (auto i = __index; i < (m_size - __n); i++)
            m_data[i] = m_data[i + __n];

        m_size -= __n;
        return m_data + __index;
    }

    void push_back(const_reference __value)
    {
        insert(end(), __value);
    }

    void push_back(value_type &&__value)
    {
        emplace(end(), __value);
    }

    template <class ..._Args>
    void emplace_back(_Args&&... __args)
    {
        emplace(end(), __args...);
    }

    void pop_back()
    {
        erase(end());
    }

    void push_front(const reference __value)
    {
        insert(begin(), __value);
    }

    void push_front(value_type &&__value)
    {
        emplace(begin(), __value);
    }

    template <class ..._Args>
    void emplace_front(_Args&&... __args)
    {
        emplace(begin(), __args...);
    }

    void pop_front()
    {
        erase(begin());
    }

    void resize(size_type __n, const_reference __value = value_type())
    {
        reserve(m_size + __n);

        for (auto i = m_size; i < m_size + __n; i++)
            m_data[i] = __value;
    }

    void swap(vector<value_type> &__v) {
        if (this == &__v) return;
        std::swap<pointer>(m_data, __v.m_data);
        std::swap<size_type>(m_size, __v.m_size);
        std::swap<size_type>(m_capacity, __v.m_capacity);
    }

protected:
    size_type m_size;
    size_type m_capacity;
    _Tp *m_data;
};

} // namespace std

#endif // GML_STL