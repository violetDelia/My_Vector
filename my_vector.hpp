#pragma once
#ifndef my_vector_hpp
#define my_vector_hpp

#include "my_vector_allocator.hpp"
#include "my_vector_help.hpp"
#include "my_vector_iterator.hpp"

/**/
#include <stdexcept>
/**/

my_vector_namespace_begin

    template <typename Ty, typename Allocator = vector_allocator<Ty>>
    class vector
{
private: //private define
    using _allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Ty>;
    using _allocator_traits = std::allocator_traits<_allocator>;

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

protected: //auxilary function about constructor and destory
    constexpr pointer _fill_of_value(pointer _start, size_type _n, const_reference value)
    {
        iterator _it(vector_begin);
        while (_n > 0)
        {
            *_it = value;
            ++_it;
            --_n;
        }
        return _it.operator->();
    }

public: //constructor and destory
    constexpr vector() noexcept(noexcept(Allocator()))
        : vector_begin(0), vector_end(0), vector_end_of_storage(0),
          vector_capacity(0), vector_allocator(), vector_size(0){};

    constexpr explicit vector(const allocator_type &alloc) noexcept
        : vector_begin(0), vector_end(0), vector_end_of_storage(0),
          vector_capacity(0), vector_allocator(alloc), vector_size(0){};

    constexpr vector(size_type count, const_reference value, const allocator_type &alloc = Allocator())
        : vector_capacity(count), vector_allocator(alloc), vector_size(count)
    {
        vector_begin = vector_allocator.allocate(count);
        vector_end = _fill_of_value(vector_begin, count, value);
        vector_end_of_storage = vector_begin + vector_capacity;
    };

    constexpr explicit vector(size_type count, const Allocator &alloc = Allocator())
        : vector_allocator(alloc), vector_capacity(count), vector_size(count)
    {
        vector_begin = vector_allocator.allocate(count);
        vector_end = vector_begin;
        vector_end_of_storage = vector_begin + vector_capacity;
    };

    /*template <class InputIt>
    constexpr vector(InputIt first, InputIt last,
                     const Allocator &alloc = Allocator());
    
    /*constexpr vector(const vector &other);
    constexpr vector(const vector &other, const Allocator &alloc);
    constexpr vector(vector &&other) noexcept;

    constexpr vector(vector &&other, const Allocator &alloc);
    constexpr vector(std::initializer_list<Ty> init,
                     const Allocator &alloc = Allocator());*/
    Constexpr ~vector()
    {

        vector_allocator.deallocate(vector_begin, vector_capacity);
    };

protected: //member
    allocator_type vector_allocator;
    pointer vector_begin;
    pointer vector_end;
    pointer vector_end_of_storage;
    size_type vector_size;
    size_type vector_capacity;

public:
    constexpr allocator_type get_allocator() const noexcept
    {
        return vector_allocator;
    };

    constexpr void assign(size_type count, const value_type &value);

public: //iterator
    constexpr iterator begin() noexcept
    {
        return iterator(vector_begin);
    };

    constexpr const_iterator begin() const noexcept
    {
        return const_iterator(vector_begin);
    }

    constexpr const_iterator cbegin() const noexcept
    {
        return const_iterator(vector_begin);
    };

    constexpr iterator end() noexcept
    {
        return iterator(vector_end);
    };

    constexpr const_iterator end() const noexcept
    {
        return const_iterator(vector_end);
    };

    constexpr const_iterator cend() const noexcept
    {
        return const_iterator(vector_end);
    };

    constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(vector_end);
    };

    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(vector_end);
    };

    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(vector_end);
    };

    constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(vector_begin);
    };

    constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(vector_begin);
    };

    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(vector_begin);
    };

public: //acess element
    constexpr reference at(size_type position);
    constexpr const_reference at(size_type position) const;
    constexpr reference operator[](size_type position);
    constexpr const_reference operator[](size_type position) const;
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;
    constexpr const_pointer data() noexcept;

protected: //auxilary function about size and capacity
    void _reallocate(const size_type _new_cap)
    {
        const size_type _real_new_cap = _get_new_capacity(_new_cap);
        const size_type _size = size();
        const pointer _new_begin = vector_allocator.allocate(_real_new_cap);
        try
        {
            _copy_elements(begin(), end(), iterator(_new_begin));
        }
        catch (...)
        {
            vector_allocator.deallocate(_new_begin, _real_new_cap);
            throw;
        }
        if (vector_begin != nullptr)
        {
            _destory_all_elements_use_iterator(begin(), end());
            vector_allocator.deallocate(vector_begin, vector_capacity);

            //init paragram

            //vector_begin = pointer();
            //vector_end = pointer();
            //vector_capacity = size_type();
            //vector_end_of_storage = pointer();
            //vector_size = size_type();
        }
        vector_begin = _new_begin;
        vector_end = _new_begin + _size;
        vector_capacity = _real_new_cap;
        vector_end_of_storage = _new_begin + _real_new_cap;
    }

public: //size and capacity
    constexpr size_type size() const noexcept
    {
        return std::distance(begin(), end());
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return begin() == end();
    }

    constexpr size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    };

    constexpr size_type capacity() const noexcept
    {
        return vector_capacity;
    };

    constexpr void reserve(const size_type new_cap)
    {
        if (new_cap > capacity())
        {
            if (new_cap > max_size())
            {
                throw std::length_error("length error");
            }
            _reallocate(new_cap);
        }
    };

    constexpr void shrink_to_fit();

public:
    constexpr void clear() noexcept;
    constexpr iterator insert(const_iterator position, const_reference value);
    constexpr iterator insert(const_iterator position, reference &value);
    constexpr iterator insert(const_iterator position, size_type count, const_reference value);
    template <class Input_iterator>
    constexpr iterator insert(const_iterator position, Input_iterator begin, Input_iterator end);
    constexpr iterator insert(const_iterator position, std::initializer_list<value_type> insert_list);
    template <class... Args>
    constexpr iterator emplace(const_iterator position, Args &&...args);
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    constexpr void push_back(const_reference value);
    constexpr void push_back(reference &value);
    template <class... Args>
    constexpr reference emplace_back(Args &&...args);
    constexpr void pop_back();
    constexpr void resize(size_type count);
    constexpr void resize(size_type count, const value_type &value);
    constexpr void swap(vector &other) noexcept(/* see std::swap */ true);

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
            std::cout << " adress: " << it.operator->()
                      << " value: " << *it
                      << std::endl;
            ++it;
        }
        std::cout << std::endl;
    }
};

my_vector_namespace_end

#endif //my_vector_hpp