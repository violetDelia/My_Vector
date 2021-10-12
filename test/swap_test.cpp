

#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
#include <vector>

template <class Os, typename Ty>
Os &operator<<(Os &os, const my_vector::vector<Ty> &co)
{
    os << "{";
    for (auto const &i : co)
    {
        os << ' ' << i;
    }
    return os << " } ";
}

int main()
{
    my_vector ::vector<int> a1{1, 2, 3}, a2{4, 5};

    auto it1 = std::next(a1.begin());
    auto it2 = std::next(a2.begin());

    int &ref1 = a1.front();
    int &ref2 = a2.front();

    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
    try
    {
        a1.swap(a2);
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
    }
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';

    // 注意交换后迭代器与引用保持与其原来的元素关联，例如指向 'a1' 中值为 2 的元素的 it1 仍指向同一元素，
    // 尽管此元素被移动到 'a2' 中。


    my_vector::vector<int> alice{1, 2, 3};
    my_vector::vector<int> bob{7, 8, 9, 10};
 
    auto print = [](const int& n) { std::cout << " " << n; };
 
    // 打印交换前的状态
    std::cout << "alice:";
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << '\n';
    std::cout << "bob  :";
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';
 
    std::cout << "-- SWAP\n";
    std::swap(alice,bob);
 
    // 打印交换后的状态
    std::cout << "alice:";
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << '\n';
    std::cout << "bob  :";
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';
    std::system("pause");
}