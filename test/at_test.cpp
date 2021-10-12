#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>

int main()
{
    my_vector::vector<int> data = {1, 2, 4, 5, 5, 6};

    // Set element 1
    data.at(1) = 88;

    // Read element 2
    std::cout << "Element at index 2 has value " << data.at(2) << '\n';

    std::cout << "data size = " << data.size() << '\n';

    try
    {
        // Set element 6
        data.at(6) = 666;
    }
    catch (std::out_of_range const &exc)
    {
        std::cout << exc.what() << '\n';
    }

    // Print final values
    std::cout << "data:";
    for (int elem : data)
        std::cout << " " << elem;
    std::cout << '\n';
    std::system("pause");
}