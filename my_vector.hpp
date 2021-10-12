#pragma once
#ifndef my_vector_hpp
#define my_vector_hpp

#include <my_vector_allocator.hpp>
#include <my_vector_help.hpp>
#include <my_vector_iterator.hpp>

/**/
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
/**/

my_vector_namespace_begin

    template <typename Ty, typename Allocator = vector_allocator<Ty>>
    class vector
{
private: //private define
    using _allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Ty>;
    using _allocator_traits = std::allocator_traits<_allocator>;
    using _no_const_value_type = typename std::remove_const_t<Ty>;

public: // common define
    using value_type = Ty;
    using allocator_type = Allocator;
    using pointer = typename _allocator_traits::pointer;
    using const_pointer = typename _allocator_traits::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = typename _allocator_traits::size_type;
    using difference_type = typename _allocator_traits::difference_type;
    using iterator = vector_iterator<vector>;
    using const_iterator = vector_const_iterator<vector>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private: //auxilary function about constructor and destory
    constexpr void _vector_reallocate_and_copy_to_new_memory(const size_type _new_cap)
    {
        const size_type _size = size();
        const pointer _new_begin = vector_allocator.allocate(_new_cap);
        iterator _begin_iterator = begin();
        iterator _end_iterator = end();
        iterator _new_begin_iterator(_new_begin);
        const pointer _new_end = _construct_all_elements_use_iterator(_begin_iterator, _end_iterator, _new_begin_iterator).operator->();
        _clear_and_deallocate();
        vector_begin = _new_begin;
        vector_end = _new_end;
        vector_size = _size;
        vector_capacity = _new_cap;
        vector_end_of_storage = _new_begin + _new_cap;
    }

    constexpr void _vector_reallocate(const size_type _new_cap)
    {
        const pointer _new_begin = vector_allocator.allocate(_new_cap);
        _clear_and_deallocate();
        vector_begin = _new_begin;
        vector_size = 0;
        vector_end = _new_begin;
        vector_capacity = _new_cap;
        vector_end_of_storage = _new_begin + vector_capacity;
    }

    constexpr void _move_copy_without_allocate(vector &&other)
    {
        vector_begin = other.begin().operator->();
        vector_end = other.end().operator->();
        vector_capacity = other.capacity();
        vector_size = other.size();
        vector_end_of_storage = vector_begin + vector_capacity;
    }

    constexpr void _after_move_copy_init_without_allocate()
    {
        vector_begin = pointer();
        vector_end = pointer();
        vector_size = size_type();
        vector_capacity = size_type();
        vector_end_of_storage = pointer();
    }

    constexpr void _clear_and_deallocate()
    {
        if (vector_begin)
        {
            clear();
            vector_allocator.deallocate(vector_begin, vector_capacity);
        }
    }

public: //constructor and destory
    constexpr vector() noexcept(noexcept(Allocator()))
        : vector_begin(), vector_end(), vector_end_of_storage(),
          vector_capacity(), vector_allocator(), vector_size(){};

    constexpr explicit vector(const allocator_type &alloc) noexcept
        : vector_begin(), vector_end(), vector_end_of_storage(),
          vector_capacity(), vector_allocator(alloc), vector_size(){};

    constexpr vector(size_type count, const_reference value, const allocator_type &alloc = Allocator())
        : vector_allocator(alloc), vector_capacity(count), vector_size(count)
    {
        vector_begin = vector_allocator.allocate(count);
        vector_end_of_storage = vector_begin + vector_capacity;
        iterator _begin_iterator = begin();
        vector_end = _construct_all_elements_use_iterator(_begin_iterator, count, value).operator->();
    };

    constexpr explicit vector(size_type count, const Allocator &alloc = Allocator())
        : vector_allocator(alloc), vector_capacity(count), vector_size(count)
    {
        vector_begin = vector_allocator.allocate(count);
        iterator _begin_iterator = begin();
        vector_end = _construct_default_all_elements_use_iterator(_begin_iterator, count).operator->();
        vector_end_of_storage = vector_begin + vector_capacity;
    };

    template <typename Forward_iterator, std::enable_if_t<_is_iterator_v<Forward_iterator>, int> = 0>
    constexpr vector(Forward_iterator init_begin, Forward_iterator init_end, const allocator_type &alloc = Allocator())
        : vector_allocator(alloc)
    {
        auto _count = std::distance(init_begin, init_end);
        vector_begin = vector_allocator.allocate(_count);
        vector_capacity = _count;
        vector_size = _count;
        vector_end_of_storage = vector_begin + vector_capacity;
        iterator _begin_iterator = begin();
        vector_end = _construct_all_elements_use_iterator(init_begin, init_end, _begin_iterator).operator->();
    };

    constexpr vector(const vector &other)
        : vector_allocator(other.get_allocator()), vector_capacity(other.capacity()), vector_size(other.size())
    {
        vector_begin = vector_allocator.allocate(other.capacity());
        vector_end_of_storage = vector_begin + vector_capacity;
        iterator _begin_iterator = begin();
        auto _other_begin_iterator = other.begin();
        auto _other_end_iterator = other.end();
        vector_end = _construct_all_elements_use_iterator(_other_begin_iterator, _other_end_iterator, _begin_iterator).operator->();
    };

    constexpr vector(const vector &other, const Allocator &alloc)
        : vector_allocator(alloc), vector_capacity(other.capacity()), vector_size(other.size())
    {
        vector_begin = vector_allocator.allocate(other.capacity());
        vector_end_of_storage = vector_begin + vector_capacity;
        iterator _begin_iterator = begin();
        auto _other_begin_iterator = other.begin();
        auto _other_end_iterator = other.end();
        vector_end = _construct_all_elements_use_iterator(_other_begin_iterator, _other_end_iterator, _begin_iterator).operator->();
    };

    constexpr vector(vector &&other) noexcept
        : vector_allocator(std::move(other.vector_allocator))
    {
        _move_copy_without_allocate(std::forward<vector>(other));
        other._after_move_copy_init_without_allocate();
    };

    constexpr vector(vector &&other, const Allocator &alloc)
        : vector_allocator(alloc)
    {
        _move_copy_without_allocate(other);
        other._after_move_copy_without_allocate_init();
    };

    constexpr vector(std::initializer_list<Ty> init_list, const allocator_type &alloc = Allocator())
        : vector_allocator(alloc)
    {
        size_type _count = init_list.size();
        vector_begin = vector_allocator.allocate(_count);
        vector_capacity = _count;
        vector_size = _count;
        vector_end_of_storage = vector_begin + vector_capacity;
        iterator _begin_iterator = begin();
        vector_end = _construct_all_elements_use_iterator(_begin_iterator, init_list).operator->();
    };

    constexpr vector &operator=(const vector &other)
    {
        if (this != std::addressof(other))
        {
            if constexpr (_allocate_propagate_on_container_copy_assignment<allocator_type>::value)
            {
                if (vector_allocator != other.vector_allocator)
                {
                    _clear_and_deallocate();
                }
                vector_allocator = other.vector_allocator;
            }
            assign(other.begin(), other.end());
        }
        return *this;
    };

    constexpr vector &operator=(vector &&other) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
        std::allocator_traits<Allocator>::is_always_equal::value)
    {
        if (this != std::addressof(other))
        {
            if constexpr (_allocate_propagate_on_container_move_assignment<allocator_type>::value)
            {
                _clear_and_deallocate();
                vector_allocator = std::move(other.vector_allocator);
                _move_copy_without_allocate(std::forward<vector>(other));
            }
            else
            {
                if (vector_allocator != other.vector_allocator)
                {
                    _clear_and_deallocate();
                    vector_allocator = other.vector_allocator;
                    vector_begin = vector_allocator.allocate(other.capacity());
                    auto _other_begin_iterator = other.begin();
                    auto _other_end_iterator = other.end();
                    iterator _begin_iterator = begin();
                    vector_end = _move_construct_all_elements_use_iterator(_other_begin_iterator, _other_end_iterator, _begin_iterator).operator->();
                    vector_size = other.size();
                    vector_capacity = other.capacity();
                    vector_end_of_storage = vector_begin + vector_capacity;
                    other._clear_and_deallocate();
                }
                else
                {
                    _clear_and_deallocate();
                    vector_allocator = other.vector_allocator;
                    _move_copy_without_allocate(std::forward<vector>(other));
                }
            }
        }
        other._after_move_copy_init_without_allocate();
        return *this;
    };

    constexpr vector &operator=(std::initializer_list<value_type> init_list)
    {
        assign(init_list);
        return *this;
    };

    Constexpr ~vector()
    {
        iterator _begin_iterator = begin();
        iterator _end_iterator = end();
        _destory_all_elements_use_iterator(_begin_iterator, _end_iterator);
        vector_allocator.deallocate(vector_begin, vector_capacity);
    };

public:
    //private: //member
    allocator_type vector_allocator;
    pointer vector_begin;
    pointer vector_end;
    pointer vector_end_of_storage;
    size_type vector_size;
    size_type vector_capacity;

public:
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept
    {
        return vector_allocator;
    };

    constexpr void assign(size_type count, const_reference value)
    {
        if (count > size())
        {
            if (count > max_size())
            {
                throw std::length_error("length_error");
            }
            const size_type _real_count = _get_new_capacity(count);
            _vector_reallocate(_real_count);
            auto _begin_iterator = begin();
            vector_end = _construct_all_elements_use_iterator(_begin_iterator, count, value).operator->();
            vector_size = count;
        }
        else
        {
            const pointer _old_end = vector_end;
            iterator _begin_iterator = begin();
            vector_end = _construct_all_elements_use_iterator(_begin_iterator, count, value).operator->();
            iterator _end_iterator = end();
            iterator _old_end_iterator = iterator(_old_end);
            //_destory_all_elements_use_iterator(_end_iterator, _old_end_iterator);
            vector_size = count;
        }
    };

    template <class Forward_iterator>
    constexpr void assign(Forward_iterator assign_begin, Forward_iterator assign_end)
    {
        const size_type _count = std::distance(assign_begin, assign_end);
        if (_count > size())
        {
            if (_count > max_size())
            {
                throw std::length_error("length_error");
            }
            const size_type _real_count = _get_new_capacity(_count);
            _vector_reallocate(_real_count);
            iterator _begin_iterator = begin();
            vector_end = _construct_all_elements_use_iterator(assign_begin, assign_end, _begin_iterator).operator->();
            vector_size = _count;
        }
        else
        {
            const pointer _old_end = vector_end;
            iterator _begin_iterator = begin();
            vector_end = _construct_all_elements_use_iterator(assign_begin, assign_end, _begin_iterator).operator->();
            iterator _end_iterator = end();
            iterator _old_end_iterator = iterator(_old_end);
            _destory_all_elements_use_iterator(_end_iterator, _old_end_iterator);
            vector_size = _count;
        }
    };

    constexpr void assign(std::initializer_list<value_type> assign_list)
    {
        const size_type _count = assign_list.size();
        if (_count > size())
        {
            if (_count > max_size())
            {
                throw std::length_error("length_error");
            }
            const size_type _real_count = _get_new_capacity(_count);
            _vector_reallocate(_count);
            iterator _begin_iterator = begin();
            vector_end = _construct_all_elements_use_iterator(_begin_iterator, assign_list).operator->();
            vector_size = _count;
        }
        else
        {
            const pointer _old_end = vector_end;
            auto _assign_list_begin_iterator = assign_list.begin();
            auto _assign_list_end_iterator = assign_list.end();
            iterator _begin_iterator = begin();
            vector_end = _construct_all_elements_use_iterator(_assign_list_begin_iterator, _assign_list_end_iterator, _begin_iterator).operator->();
            iterator _end_iterator = end();
            iterator _old_end_iterator = iterator(_old_end);
            _destory_all_elements_use_iterator(_end_iterator, _old_end_iterator);
            vector_size = _count;
        }
    };

public: //iterator
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return iterator(vector_begin);
    };

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return const_iterator(vector_begin);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return const_iterator(vector_begin);
    };

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return iterator(vector_end);
    };

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return const_iterator(vector_end);
    };

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return const_iterator(vector_end);
    };

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(vector_end);
    };

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(vector_end);
    };

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(vector_end);
    };

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(vector_begin);
    };

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(vector_begin);
    };

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(vector_begin);
    };

public: //acess element
    [[nodiscard]] constexpr reference at(size_type position)
    {
        if (position >= size())
        {
            throw std::out_of_range("out of range");
        }
        return vector_begin[position];
    };

    [[nodiscard]] constexpr const_reference at(size_type position) const
    {
        if (position >= size())
        {
            throw std::out_of_range("out of range");
        }
        return vector_begin[position];
    };

    [[nodiscard]] constexpr reference operator[](size_type position)
    {
        //return *(begin() + position);
        return vector_begin[position];
    };

    [[nodiscard]] constexpr const_reference operator[](size_type position) const
    {
        //return *(cbegin() + position);
        return vector_begin[position];
    };

    [[nodiscard]] constexpr reference front()
    {
        return *vector_begin;
    };

    [[nodiscard]] constexpr const_reference front() const
    {
        return *vector_begin;
    };

    [[nodiscard]] constexpr reference back()
    {
        return vector_end[-1];
    };

    [[nodiscard]] constexpr const_reference back() const
    {
        return vector_end[-1];
    };

    [[nodiscard]] constexpr const_pointer data() noexcept
    {
        return _get_real_offointer(vector_begin);
    };

public: //size and capacity
    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return vector_size;
        //return std::distance(begin(), end());
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return begin() == end();
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    };

    [[nodiscard]] constexpr size_type capacity() const noexcept
    {
        return vector_capacity;
    };

    constexpr void reserve(const size_type new_cap)
    {
        if (new_cap > capacity())
        {
            const size_type _real_new_cap = _get_new_capacity(new_cap);
            if (_real_new_cap > max_size())
            {
                throw std::length_error("length error");
            }
            _vector_reallocate_and_copy_to_new_memory(_real_new_cap);
        }
    };

    constexpr void shrink_to_fit()
    {
        _vector_reallocate_and_copy_to_new_memory(size());
    };

public:
    constexpr void clear() noexcept
    {
        iterator _begin_iterator = begin();
        iterator _end_iterator = end();
        _destory_all_elements_use_iterator(_begin_iterator, _end_iterator);
        vector_end = vector_begin;
        vector_size = 0;
    };

    constexpr iterator insert(const_iterator position, const_reference value)
    {
        if (position <= cend())
        {
            return emplace(position, value);
        }
        return iterator(nullptr);
    };

    constexpr iterator insert(const_iterator position, value_type &&value)
    {
        if (position <= cend())
        {
            return emplace(position, std::move(value));
        }
        return iterator(nullptr);
    };

    constexpr iterator insert(const_iterator position, size_type count, const_reference value)
    {
        const size_type _off = position - cbegin();
        reserve(size() + count);
        if (cend() != cbegin())
        {
            iterator _rear_iterator = end() - 1;
            iterator _begin_iterator = begin() + _off;
            _move_back_spaces_uncheck(_begin_iterator, _rear_iterator, count);
        }
        iterator _position_iterator = begin() + _off;
        _construct_all_elements_use_iterator(_position_iterator, count, value);
        vector_size += count;
        vector_end += count;
        return _position_iterator;
    };

    template <typename Input_iterator, std::enable_if_t<_is_iterator_v<Input_iterator>, int> = 0>
    constexpr iterator insert(const_iterator position, Input_iterator insert_begin, Input_iterator insert_end)
    {
        if (insert_begin > insert_end)
            return iterator(nullptr);
        const size_type _off = position - cbegin();
        const auto _count = std::distance(insert_begin, insert_end);
        reserve(size() + _count);
        if (cend() != cbegin())
        {
            iterator _rear_iterator = end() - 1;
            iterator _begin_iterator = begin() + _off;
            _move_back_spaces_uncheck(_begin_iterator, _rear_iterator, _count);
        }
        iterator _position_iterator = begin() + _off;
        _construct_all_elements_use_iterator(insert_begin, insert_end, _position_iterator);
        vector_size += _count;
        vector_end += _count;
        return _position_iterator;
    };

    constexpr iterator insert(const_iterator position, std::initializer_list<value_type> insert_list)
    {
        insert(position, insert_list.begin(), insert_list.end());
    };

    template <class... Args>
    constexpr iterator emplace(const_iterator position, Args &&...args)
    {
        const size_type _off = position - cbegin();
        reserve(size() + 1);
        if (cbegin() + _off <= cend())
        {
            emplace_back(std::forward<Args>(args)...);
            iterator _rear_iterator = end() - 1;
            iterator _real_offosition = begin() + _off;
            return _move_back_one_space_and_insert_to(_real_offosition, _rear_iterator);
            //return iterator(nullptr);
        }
        iterator _real_offosition = begin() + _off;
        _allocator_traits::construct(vector_allocator, _get_real_pointer(_real_offosition.operator->()), std::forward<Args>(args)...);
        ++vector_size;
        return _real_offosition;
    };

    constexpr iterator erase(const_iterator position)
    {
        if (position < cbegin() || position >= cend())
            return iterator(nullptr);
        iterator _rear_iterator = end() - 1;
        iterator _position_iterator(position.operator->());
        _move_forward_spaces_uncheck(_position_iterator, _rear_iterator, 1);
        pop_back();
        return iterator(position.operator->());
    };

    constexpr iterator erase(const_iterator erase_begin, const_iterator erase_end)
    {
        if (erase_begin >= erase_end)
        {
            return iterator(nullptr);
        }
        if (erase_begin < cbegin())
        {
            return iterator(nullptr);
        }
        const_iterator _const_old_end_iterator = cend();
        if (erase_end > _const_old_end_iterator)
        {
            erase_end = _const_old_end_iterator;
        }
        const size_type _count = std::distance(erase_begin, erase_end);
        iterator _position_iterator = iterator(erase_begin.operator->());
        iterator _rear_iterator = end() - 1;
        _move_forward_spaces_uncheck(_position_iterator, _rear_iterator, _count);
        vector_size -= _count;
        iterator _old_end_iterator = end();
        vector_end = (_old_end_iterator - _count).operator->();
        iterator _new_end_iterator = end();
        _destory_all_elements_use_iterator(_new_end_iterator, _old_end_iterator);
        return iterator(erase_begin.operator->());
    };

    constexpr void push_back(const_reference value)
    {
        emplace_back(value);
    };

    constexpr void push_back(value_type &&value)
    {
        emplace_back(std::move(value));
    };

    template <class... Args>
    constexpr reference emplace_back(Args &&...args)
    {
        reserve(size() + 1);
        pointer _last_end = vector_end;
        _allocator_traits::construct(vector_allocator, _get_real_pointer(_last_end), std::forward<Args>(args)...);
        ++vector_end;
        ++vector_size;
        return *_last_end;
    };

    constexpr void pop_back()
    {
        if (empty())
            return;
        const_iterator _rear_iterator = end() - 1;
        _destory_a_element_use_iterator(_rear_iterator);
        --vector_size;
        vector_end = _rear_iterator.operator->();
    };

    constexpr void resize(size_type count)
    {
        if (count > capacity())
        {
            _vector_reallocate_and_copy_to_new_memory(count);
            iterator _old_end_iterator = end();
            vector_end = _construct_default_all_elements_use_iterator(_old_end_iterator, count - size()).operator->();
            vector_size = count;
        }
        else
        {
            vector_size = count;
            vector_end = vector_begin + count;
        }
    };

    constexpr void resize(size_type count, const value_type &value)
    {
        if (count > capacity())
        {
            _vector_reallocate_and_copy_to_new_memory(count);
            iterator _old_end_iterator = end();
            vector_end = _construct_all_elements_use_iterator(_old_end_iterator, count - size(), value).operator->();
            vector_size = count;
        }
        else
        {
            vector_size = count;
            vector_end = vector_begin + count;
        }
    };

    constexpr void swap(vector &other) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
        std::allocator_traits<Allocator>::is_always_equal::value)
    {
        if constexpr (_allocate_propagate_on_container_swap<allocator_type>::value)
        {
            const allocator_type _temp_allocator = std::move(other.vector_allocator);
            other.vector_allocator = std::move(vector_allocator);
            vector_allocator = std::move(_temp_allocator);
        }
        else
        {
            if (vector_allocator != other.vector_allocator)
                Undefined_Activity return; //throw std::exception("undefined action");
        }
        _swap(vector_begin, other.vector_begin);
        _swap(vector_capacity, other.vector_capacity);
        _swap(vector_end, other.vector_end);
        _swap(vector_end_of_storage, other.vector_end_of_storage);
        _swap(vector_size, other.vector_size);
    };

public: //test
    void print_member_info()
    {
        std::cout << " this: " << std::addressof(*this)
                  << " size: " << sizeof(*this)
                  << std::endl;
        std::cout << " adress: " << std::addressof(vector_allocator)
                  << " size: " << sizeof(vector_allocator)
                  << " vector_allocator"
                  << std::endl;
        std::cout << " adress: " << std::addressof(vector_begin)
                  << " size: " << sizeof(vector_begin)
                  << " value: " << vector_begin
                  << " vector_begin"
                  << std::endl;
        std::cout << " adress: " << std::addressof(vector_end)
                  << " size: " << sizeof(vector_end)
                  << " value: " << vector_end
                  << " vector_end"
                  << std::endl;
        std::cout << " adress: " << std::addressof(vector_end_of_storage)
                  << " size: " << sizeof(vector_end_of_storage)
                  << " value: " << vector_end_of_storage
                  << " vector_end_of_storage"
                  << std::endl;
        std::cout << " adress: " << std::addressof(vector_size)
                  << " size: " << sizeof(vector_size)
                  << " value: " << vector_size
                  << " vector_size"
                  << std::endl;
        std::cout << " adress: " << std::addressof(vector_capacity)
                  << " size: " << sizeof(vector_capacity)
                  << " value: " << vector_capacity
                  << " vector_capacity"
                  << std::endl
                  << std::endl;
    }

    void print_capacity()
    {
        iterator it = begin();
        const_iterator it_end = cend();
        while (it != it_end)
        {
            std::cout << " adress: " << it.operator->();
            if constexpr (std::is_integral<value_type>::value || std::is_floating_point<value_type>::value)
            {
                std::cout << " value: " << *it;
            }
            std::cout << std::endl;
            ++it;
        }
        std::cout << std::endl;
    }
};

template <class Ty, class Alloc>
constexpr void swap(vector<Ty, Alloc> &left_vector, vector<Ty, Alloc> &right_vector) noexcept(
    noexcept(noexcept(left_vector.swap(right_vector))))
{
    left_vector.swap(right_vector);
};

template <class Ty, class Alloc, class U>
constexpr typename vector<Ty, Alloc>::size_type erase(vector<Ty, Alloc> &vector, const U &value)
{
    auto it = std::remove(vector.begin(), vector.end(), value);
    auto erased_count = std::distance(it, vector.end());
    vector.erase(it, vector.end());
    return erased_count;
};

template <class Ty, class Alloc, class Pred>
constexpr typename vector<Ty, Alloc>::size_type
erase_if(vector<Ty, Alloc> &vector, Pred pred)
{
    auto it = std::remove_if(vector.begin(), vector.end(), pred);
    auto erased_count = std::distance(it, vector.end());
    vector.erase(it, vector.end());
    return erased_count;
};
my_vector_namespace_end

#endif //my_vector_hpp