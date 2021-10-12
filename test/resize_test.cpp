#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    my_vector::vector<int> c = {1, 2, 3};
    std::cout << "The vector holds: ";
    for(auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
    c.resize(5,6);
    std::cout << "After resize up to 5: ";
    for(auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
    c.resize(2);
    std::cout << "After resize down to 2: ";
    for(auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
    std::system("pause");
}