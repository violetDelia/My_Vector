#pragma once
#ifndef my_vector_iterator_hpp
#define my_vector_iterator_hpp

#include <my_vector_help.hpp>
/**/
#include <iterator>
/**/
my_vector_namespace_begin

    template <typename Vector>
    class vector_const_iterator
{
private:
    using _address = typename Vector ::pointer;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = const value_type &;

private:
    _address element_pointer;

public:
    vector_const_iterator(_address ptr) : element_pointer(ptr){};

public:
    [[nodiscard]] reference operator*() const
    {
        return *element_pointer;
    }

    [[nodiscard]] _address operator->() const
    {
        return element_pointer;
    }

    vector_const_iterator &operator++()
    {
        ++element_pointer;
        return *this;
    }

    vector_const_iterator &operator++(int)
    {
        vector_const_iterator _temp = *this;
        ++element_pointer;
        return _temp;
    }

    vector_const_iterator &operator--()
    {
        --element_pointer;
        return *this;
    }

    vector_const_iterator &operator--(int)
    {
        vector_const_iterator _temp = *this;
        --element_pointer;
        return _temp;
    }

    vector_const_iterator &operator+=(const difference_type n)
    {
        element_pointer += n;
        return *this;
    }

    [[nodiscard]] vector_const_iterator operator+(const difference_type n) const
    {
        vector_const_iterator _temp = *this;
        return _temp += n;
    }

    vector_const_iterator &operator-=(const difference_type n)
    {
        element_pointer -= n;
        return *this;
    }

    [[nodiscard]] vector_const_iterator operator-(const difference_type n) const
    {
        vector_const_iterator _temp = *this;
        return _temp -= n;
    }

    [[nodiscard]] difference_type operator-(const vector_const_iterator &left) const
    {
        return element_pointer - left.element_pointer;
    }

    [[nodiscard]] reference operator[](const difference_type n) const
    {
        return *(*this + n);
    }

    [[nodiscard]] bool operator==(const vector_const_iterator &other) const
    {
        return element_pointer == other.element_pointer;
    }

    [[nodiscard]] bool operator!=(const vector_const_iterator &other) const
    {
        return element_pointer != other.element_pointer;
    }

    [[nodiscard]] bool operator<(const vector_const_iterator &other) const
    {
        return element_pointer < other.element_pointer;
    }

    [[nodiscard]] bool operator>(const vector_const_iterator &other) const
    {
        return element_pointer > other.element_pointer;
    }

    [[nodiscard]] bool operator>=(const vector_const_iterator &other) const
    {
        return element_pointer >= other.element_pointer;
    }

    [[nodiscard]] bool operator<=(const vector_const_iterator &other) const
    {
        return element_pointer <= other.element_pointer;
    }
};

template <typename Vector>
class vector_iterator : public vector_const_iterator<Vector>
{
private:
    using const_iterator = vector_const_iterator<Vector>;
    using const_iterator::const_iterator;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::pointer;
    using reference = value_type &;

public:
    [[nodiscard]] reference operator*() const
    {
        return const_cast<reference>(const_iterator::operator*());
    }

    [[nodiscard]] pointer operator->() const
    {
        return const_iterator::operator->();
    }

    vector_iterator &operator++()
    {
        const_iterator::operator++();
        return *this;
    }

    vector_iterator &operator++(int)
    {
        vector_iterator _temp = *this;
        const_iterator::operator++();
        return _temp;
    }

    vector_iterator &operator--()
    {
        const_iterator::operator--();
        return *this;
    }

    vector_iterator &operator--(int)
    {
        vector_iterator _temp = *this;
        const_iterator::operator--();
        return _temp;
    }

    vector_iterator &operator+=(const difference_type n)
    {
        const_iterator::operator+=(n);
        return *this;
    }

    [[nodiscard]] vector_iterator operator+(const difference_type n) const
    {
        vector_iterator _temp = *this;
        return _temp += n;
    }

    vector_iterator &operator-=(const difference_type n)
    {
        const_iterator::operator-=(n);
        return *this;
    }

    [[nodiscard]] vector_iterator operator-(const difference_type n) const
    {
        vector_iterator _temp = *this;
        return _temp -= n;
    }

    [[nodiscard]] difference_type operator-(const vector_iterator &other) const
    {
        return const_iterator::operator-(other);
    }

    [[nodiscard]] reference operator[](const difference_type n) const
    {
        return const_cast<reference>(const_iterator::operator[](n));
    }
};

my_vector_namespace_end
#endif //my_vector_iterator_hpp