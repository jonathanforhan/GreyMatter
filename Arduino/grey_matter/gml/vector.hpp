#pragma once

#if GML_STL

#include "iterator.hpp"
#include "initializer_list.hpp"

#pragma GCC system_header
#pragma GCC visibility push(default)
#pragma pack(push, 8)

namespace std {

template <class T, class Alloc>
class vector
{
public:
    using reference                 = typename Alloc::reference;
    using const_reference           = typename Alloc::const_reference;
    using size_type                 = typename Alloc::size_type;
    using difference_type           = typename Alloc::difference_type;
    using pointer                   = typename Alloc::pointer;
    using const_pointer             = typename Alloc::pointer;

    using iterator                  = T*;
	using const_interator           = const T*;
	using value_type                = T;
	using allocator_type            = Alloc;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

    explicit vector(const Alloc &__al = Alloc())
          : m_data(nullptr),
            m_size(0),
            m_capacity(1),
            m_allocator(__al)
    {
        m_data = __al.allocate(m_capacity);
    }

    explicit vector(size_type __n, const T &__value = T(), const Alloc &__al = Alloc())
          : m_data(nullptr),
            m_size(0),
            m_capacity(0),
            m_allocator(__al) {}

protected:
    T *m_data;
    size_type m_size;
    size_type m_capacity;
    Alloc m_allocator;
};

} // namespace std

#pragma pack(pop)
#pragma GCC visibility pop

#endif // GML_STL