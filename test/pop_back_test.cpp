#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
template <class T>
void print(T const &xs)
{
    std::cout << "[ ";
    for (auto &&x : xs)
    {
        std::cout << x << ' ';
    }
    std::cout << "]\n";
}

int main()
{
    my_vector::vector<int> numbers;

    print(numbers);

    numbers.push_back(5);
    numbers.push_back(3);
    numbers.push_back(4);

    print(numbers);

    numbers.pop_back();

    print(numbers);
    std::system("pause");
}