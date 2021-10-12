#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>

struct A
{
    std::string s;
    A(std::string str) : s(std::move(str)) { std::cout << " constructed\n"; }
    A(const A &o) : s(o.s) { std::cout << " copy constructed\n"; }
    A(A &&o) : s(std::move(o.s)) { std::cout << " move constructed\n"; }
    A &operator=(const A &other)
    {
        s = other.s;
        std::cout << " copy assigned\n";
        return *this;
    }
    A &operator=(A &&other)
    {
        s = std::move(other.s);
        std::cout << " move assigned\n";
        return *this;
    }
};

int main()
{
    my_vector::vector<A> container;
    // 预留足够的空间以使 vector 不必重设大小
    std::cout << "construct 2 times A:\n";
    A two{"two"};
    A three{"three"};

    std::cout << "emplace:\n";
    auto itone = container.emplace(container.end(), "one");

    std::cout << "emplace with A&:\n";
    auto ittwo = container.emplace(container.begin(), two);

    std::cout << "emplace with A&&:\n";
    auto itthree = container.emplace(container.end() + 1, std::move(three));

    std::cout << "content:\n";
    for (const auto &obj : container)
        std::cout << ' ' << obj.s;
    std::cout << "\n"
              << container[3].s << "\n";
    std::cout << '\n';

    std::system("pause");
}