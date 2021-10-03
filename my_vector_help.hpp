#pragma once
#ifndef my_vector_help_hpp
#define my_vector_help_hpp

#include <cstddef>
#include <type_traits>
#include <utility>

#define my_vector_namespace my_vector
#define M_mat my_vector_namespace::
#define my_vector_namespace_begin \
    namespace my_vector_namespace \
    {
#define my_vector_namespace_end }

my_vector_namespace_begin

    using ::size_t; //cstddef
using ::ptrdiff_t;  //cstddef

//get the power of two that not less than _new_cap
template <typename size_type>
[[nodiscard]] constexpr size_type _get_new_capacity(const size_type _new_cap)
{
    size_type _real_new_cap = _new_cap;
    --_real_new_cap;
    int max_off_len = sizeof(size_type) * 8; //one byte correspond eight bits
    int _right_off_len = 1;
    while (_right_off_len <= max_off_len)
    {
        _real_new_cap |= (_real_new_cap >> _right_off_len);
        _right_off_len <<= 1;
    }
    return ++_real_new_cap;
}

//all elements are moved to new capacity, if object no right value assignment, will error
template <typename Input_iterator, typename Output_iterator>
constexpr void _copy_elements(Input_iterator _begin, Input_iterator _end, Output_iterator _new_begin)
{
    while (_begin != _end)
    {
        *_new_begin = std::move(*_begin);
        ++_begin;
        ++_new_begin;
    }
};

template <typename _Ty>
constexpr void _destory_a_element(_Ty &_referenKce_of_obj)
{
    _reference_of_obj.~_Ty();
};

template <typename Input_iterator>
constexpr void _destory_all_elements_use_iterator(Input_iterator _begin, Input_iterator _end)
{
    while (_begin != _end)
    {
        _destory_a_element(*_begin);
        ++_begin;
    }
};

#define Constexpr //Eliminate warning and error

my_vector_namespace_end

#endif //my_vector_help_hpp