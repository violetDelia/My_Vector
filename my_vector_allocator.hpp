#pragma once
#ifndef my_vector_allocator_hpp
#define my_vector_allocator_hpp

#include <my_vector_help.hpp>

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
    if (show_allocate_and_deallocate_log)
    {

        _Ty *_adress = nullptr;
        std::cout << "do allocate called: " << std::endl;
        std::cout << "  allocate number: " << n << std::endl
                  << "  number size: " << sizeof(_Ty) << std::endl;
        if (_do_allocate_lenth == 0)
        {
            std::cout << "  allocate adress: " << _adress << std::endl
                      << "  allocate size:" << _do_allocate_lenth << std::endl
                      << std::endl;
        }
        else
        {
            _adress = static_cast<_Ty *>(::operator new(_do_allocate_lenth));
            std::cout << "  allocate adress: " << _adress << std::endl
                      << "  allocate size:" << _do_allocate_lenth << std::endl
                      << std::endl;
        }
        return _adress;
    }
    else
    {
        return _do_allocate_lenth == 0 ? nullptr : static_cast<_Ty *>(::operator new(_do_allocate_lenth));
    }
}

template <typename _Ty>
inline constexpr void _do_deallocate(_Ty *ptr, size_t n)
{
    if (show_allocate_and_deallocate_log)
    {
        std::cout << "do deallocate called: " << std::endl
                  << "  deallocate adress: " << ptr << std::endl
                  << "  deallocate size: " << n << std::endl
                  << std::endl;
    }
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
    using propagate_on_container_copy_assignment = std::true_type;
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
    vector_allocator(const vector_allocator &other) = default;
    /*{
        std::cout << "vector_alloctor copied called: " << this
                  << " size: " << sizeof(*this)
                  << std::endl
                  << std::endl;
    }*/
    vector_allocator(vector_allocator &&other) = default;

    vector_allocator &operator=(const vector_allocator &other) = default;

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

template <typename Ty, typename Other_Ty>
constexpr inline bool operator==(vector_allocator<Ty> &left, vector_allocator<Other_Ty> &right)
{
    return false;
}

template <typename Ty>
constexpr inline bool operator==(vector_allocator<Ty> &left, vector_allocator<Ty> &right)
{
    if (_allocate_is_always_equal<vector_allocator<Ty>>::value)
    {
        return true;
    }
    return std::addressof(left) == std::addressof(right);
}

template <typename Ty, typename Other_Ty>
constexpr inline bool operator!=(vector_allocator<Ty> &left, vector_allocator<Other_Ty> &right)
{
    return true;
}

template <typename Ty>
constexpr inline bool operator!=(vector_allocator<Ty> &left, vector_allocator<Ty> &right)
{
    if (_allocate_is_always_equal<vector_allocator<Ty>>::value)
    {
        return false;
    }
    return std::addressof(left) != std::addressof(right);
}

my_vector_namespace_end

#endif //my_vector_allocator_hpp