#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    my_vector ::vector<char> s;
    std::cout << "Maximum size of a 'vector' is " << s.max_size() << "\n";
    std::system("pause");
}