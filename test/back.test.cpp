#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    my_vector::vector<char> letters {'o', 'm', 'g', 'w', 't', 'f'};
    if (!letters.empty()) {
        std::cout << "The last character is: " << letters.back() << '\n';
    }  
    std::system("pause");
}