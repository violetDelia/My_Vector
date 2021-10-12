#pragma once
#ifndef my_vector_help_hpp
#define my_vector_help_hpp

#include <cstddef>
#include <iostream>
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

/******************* log *********************/
bool show_allocate_and_deallocate_log = false;
bool show_destory_log = false;
bool show_construct_log = false;
/******************* log *********************/

/*************** type help ******************/
template <class T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } // C++14
};
template <bool B>
using bool_constant = integral_constant<bool, B>;
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;
/*************** type help ******************/

/****************** iterator *****************/
template <typename Iterator>
using _iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

template <typename Ty, class = void>
constexpr inline bool _is_iterator_v = false;

template <typename Ty>
constexpr inline bool _is_iterator_v<Ty, std::void_t<_iterator_category<Ty>>> = true;

template <typename Iterator>
using _iterator_value_type = typename std::iterator_traits<Iterator>::value_type;

/****************** iterator *****************/

/****************** alocator *****************/
template <class Alloc>
using _allocate_propagate_on_container_copy_assignment =
    bool_constant<std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value>;

template <class Alloc>
using _allocate_propagate_on_container_move_assignment =
    bool_constant<std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value>;

template <class Alloc>
using _allocate_is_always_equal =
    bool_constant<std::allocator_traits<Alloc>::is_always_equal::value>;

template <class Alloc>
using _allocate_propagate_on_container_swap =
    bool_constant<std::allocator_traits<Alloc>::propagate_on_container_swap::value>;

/****************** alocator *****************/

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

template <typename Input_iterator, typename Ty>
constexpr inline void _construct_a_element_use_iterator(Input_iterator &_position, Ty &_value)
{
    if (show_construct_log)
    {
        std::cout << "construct a object: " << std::endl
                  << "  adress: " << std::addressof(*_position) << std::endl
                  << "  size " << sizeof(Ty) << std::endl
                  << std::endl;
    }
    ::new (static_cast<void *>(std::addressof(*_position))) Ty(std::forward<Ty>(_value));
}

template <typename Forward_iterator, typename Size_type, typename Value_type>
[[nodiscard]] constexpr Forward_iterator _construct_all_elements_use_iterator(
    Forward_iterator &_start, Size_type _n, Value_type &_value)
{
    while (_n > 0)
    {
        _construct_a_element_use_iterator(_start, std::forward<Value_type>(_value));
        //::new (static_cast<void *>(std::addressof(*_start))) Value_type(_value);
        //*_it = std::move(_value);
        ++_start;
        --_n;
    }
    return _start;
}

template <typename Forward_iterator, typename Output_Forward_iterator>
[[nodiscard]] constexpr Output_Forward_iterator _construct_all_elements_use_iterator(
    Forward_iterator &_begin, Forward_iterator &_end, Output_Forward_iterator &_new_begin)
{
    while (_begin != _end)
    {
        _construct_a_element_use_iterator(_new_begin, *_begin);
        ++_new_begin;
        ++_begin;
    }
    return _new_begin;
}

template <typename Forward_iterator, typename Value_type>
constexpr Forward_iterator _construct_all_elements_use_iterator(
    Forward_iterator &_start, std::initializer_list<Value_type> &_init_list)
{
    auto _init_list_iterator = _init_list.begin();
    auto _init_list_end_iterator = _init_list.end();
    while (_init_list_iterator != _init_list_end_iterator)
    {
        _construct_a_element_use_iterator(_start, *_init_list_iterator);
        //::new (static_cast<void *>(std::addressof(*_vector_iterator))) value_type(*_init_list_iterator);
        ++_start;
        ++_init_list_iterator;
    }
    return _start;
}

template <typename Ty>
constexpr inline void _construct_defalut_a_element_use_pointer(Ty *_adress)
{
    ::new (static_cast<void *>(_adress)) Ty();
}

template <typename Input_iterator>
constexpr inline void _construct_default_a_element_use_iterator(Input_iterator &_position)
{
    if (show_construct_log)
    {
        std::cout << "construct a object: " << std::endl
                  << "  adress: " << std::addressof(*_position) << std::endl
                  << std::endl;
    }
    _construct_defalut_a_element_use_pointer(_position.operator->());
}

template <typename Forward_iterator, typename Size_type>
[[nodiscard]] constexpr Forward_iterator _construct_default_all_elements_use_iterator(
    Forward_iterator &_start, Size_type _n)
{
    while (_n > 0)
    {
        _construct_default_a_element_use_iterator(_start);
        //::new (static_cast<void *>(std::addressof(*_start))) Value_type(_value);
        //*_it = std::move(_value);
        ++_start;
        --_n;
    }
    return _start;
}

template <typename Input_iterator, typename Ty>
constexpr inline void _move_construct_a_element_use_iterator(Input_iterator &_position, Ty &&_value)
{
    if (show_construct_log)
    {
        std::cout << "construct a object: " << std::endl
                  << "  adress: " << std::addressof(*_position) << std::endl
                  << "  size " << sizeof(Ty) << std::endl
                  << std::endl;
    }
    ::new (static_cast<void *>(std::addressof(*_position))) Ty(std::move(_value));
    //::new (static_cast<void *>(std::addressof(*_position))) Ty();
}

template <typename Forward_iterator, typename Output_Forward_iterator>
[[nodiscard]] constexpr Output_Forward_iterator _move_construct_all_elements_use_iterator(
    Forward_iterator &_begin, Forward_iterator &_end, Output_Forward_iterator &_new_begin)
{
    while (_begin != _end)
    {
        _move_construct_a_element_use_iterator(_new_begin, std::move(*_begin));
        ++_new_begin;
        ++_begin;
    }
    return _new_begin;
}

template <typename Forward_iterator>
constexpr Forward_iterator _move_back_one_space_and_insert_to(Forward_iterator &_position, Forward_iterator &_rear)
{
    if (_rear == _position)
    {
        return _rear;
    }
    auto _old_rear = *_rear;
    while (_rear != _position)
    {
        _move_construct_a_element_use_iterator(_rear, std::move(*(_rear - 1)));
        --_rear;
    }
    _move_construct_a_element_use_iterator(_rear, std::move(_old_rear));
    return _rear;
}

template <typename Forward_iterator>
constexpr void _move_back_spaces_uncheck(Forward_iterator &_start, Forward_iterator &_rear, size_t n)
{
    auto _target_iterator = _rear + n;
    while (_rear > _start)
    {
        _move_construct_a_element_use_iterator(_target_iterator, std::move(*_rear));
        --_rear;
        --_target_iterator;
    }
}

template <typename Forward_iterator>
constexpr void _move_forward_spaces_uncheck(Forward_iterator &_start, Forward_iterator &_rear, size_t n)
{
    _start += n;
    auto _target_iterator = _start - n;
    while (_start <= _rear)
    {
        _move_construct_a_element_use_iterator(_target_iterator, std::move(*_start));
        ++_start;
        ++_target_iterator;
    }
}

template <typename Ty>
constexpr inline void _destory_a_element(Ty &_reference_of_object)
{
    if (show_destory_log)
    {
        std::cout << "destory a objcet: " << std::endl
                  << "  adress: " << std::addressof(_reference_of_object) << std::endl
                  << "  size: " << sizeof(Ty) << std::endl
                  << std::endl;
    }
    _reference_of_object.~Ty();
};

template <typename Input_iterator>
constexpr inline void _destory_a_element_use_iterator(Input_iterator &_position)
{
    _destory_a_element(*_position);
}

template <typename Forward_iterator>
constexpr void _destory_all_elements_use_iterator(
    Forward_iterator &_begin, Forward_iterator &_end)
{
    if (_begin >= _end)
        return;
    while (_begin != _end)
    {
        _destory_a_element_use_iterator(_begin);
        ++_begin;
    }
};

template <typename Ty>
constexpr inline void _swap(Ty &other, Ty &another)
{
    Ty _temp = std::move(other);
    other = std::move(another);
    another = std::move(_temp);
}

template <class Fancy_pointer>
constexpr inline auto _get_real_pointer(Fancy_pointer ptr)
{
    return ptr ? std::addressof(*ptr) : nullptr;
}

#define Constexpr //Eliminate warning and error
#define Undefined_Activity
my_vector_namespace_end

#endif //my_vector_help_hpp