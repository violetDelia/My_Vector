#pragma once
#ifndef my_vector_allocator_hpp
#define my_vector_allocator_hpp

#include "my_vector_help.hpp"

/**/
#include <iostream>
#include <limits>
#include <memory>
/**/

my_vector_namespace_begin

    template <size_t _size_of_Ty>
    inline constexpr size_t _get_size(const size_t n)
{
    constexpr bool _overflow_posibility = _size_of_Ty > 1;
    if Constexpr (_overflow_posibility)
    {
        constexpr size_t _max_n = std::numeric_limits<size_t>::max() / _size_of_Ty;
        if (_max_n < n)
        {
            throw std::bad_array_new_length();
        }
    }
    return n * _size_of_Ty;
}

template <typename _Ty>
inline constexpr _Ty *_do_allocate(size_t n)
{
    size_t _do_allocate_lenth = _get_size<sizeof(_Ty)>(n);
    return _do_allocate_lenth == 0 ? nullptr : static_cast<_Ty *>(::operator new(_do_allocate_lenth));
}

template <typename _Ty>
inline constexpr void _do_deallocate(_Ty *ptr, size_t n)
{
    ::operator delete(ptr, n);
}

//copy of std::allocator
template <typename Ty>
class vector_allocator
{
public:
    using value_type = Ty;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;
    using point = Ty *;

public:
    vector_allocator() = default;
    /*{
        std::cout << "vector_allocator constructor called: " << this
                  << " size: " << sizeof(*this)
                  << std::endl
                  << std::endl;
    }*/
    vector_allocator(const vector_allocator &alloc) = default;
    /*{
        std::cout << "vector_alloctor copied called: " << this
                  << " size: " << sizeof(*this)
                  << std::endl
                  << std::endl;
    }*/

    ~vector_allocator() = default;
    /*{
        std::cout << "vector_alloctor destroied : " << this
                  << " size: " << sizeof(*this)
                  << std::endl
                  << std::endl;
    }*/

public:
    [[nodiscard]] constexpr point allocate(size_t n)
    {
        return _do_allocate<value_type>(n);
    };

    void constexpr deallocate(point ptr, size_t n)
    {
        _do_deallocate<value_type>(ptr, n * sizeof(value_type));
    };
};

my_vector_namespace_end

#endif //my_vector_allocator_hpp