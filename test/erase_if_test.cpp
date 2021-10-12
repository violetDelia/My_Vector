#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>

void print_container(const my_vector::vector<char> &c)
{
    for (auto x : c)
    {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

int main()
{
    my_vector::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');

    std::cout << "Init:\n";
    print_container(cnt);

    auto erased = my_vector::erase(cnt, '3');
    std::cout << "Erase \'3\':\n";
    print_container(cnt);

    my_vector::erase_if(cnt, [](char x)
                        { return (x - '0') % 2 == 0; });
    std::cout << "Erase all even numbers:\n";
    print_container(cnt);
    std::cout << "In all " << erased << " even numbers were erased.\n";
    std::system("pause");
}