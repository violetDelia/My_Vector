#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    my_vector::vector<std::string> c{"0", " 1", " 2", "3", "4", "5", "6", "7", "8", "9"};
    for (auto &i : c)
    {
        std::cout << i << " ";
    }
    /*std::cout << '\n';

    std::cout << '\n'
              << *(c.erase(c.begin() + 8)) << '\n';

    for (auto &i : c)
    {
        std::cout << i << " ";
    }*/
    std::cout << '\n';
    std::cout
        << *c.erase(c.begin(), c.begin() + 12)
        << '\n';
    for (auto &i : c)
    {
        std::cout << i << " ";
    }
    std::cout << '\n';
    std::system("pause");
}