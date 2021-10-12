#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{ 
    my_vector::vector<int> nums {1, 3, 5, 7};
 
    std::cout << "nums contains " << nums.size() << " elements.\n";
    std::system("pause");
}