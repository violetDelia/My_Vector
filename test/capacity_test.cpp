#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    int sz = 200;
    my_vector::vector<int> v1;
 
    auto cap = v1.capacity();
    std::cout << "initial capacity=" << cap << '\n';
 
    for (int n = 0; n < sz; ++n) {
        //v1.push_back(n);
        if (cap != v1.capacity()) {
            cap = v1.capacity();
            std::cout << "new capacity=" << cap << '\n';
        }
    }
 
    std::cout << "final size=" << v1.size() << '\n';
    std::cout << "final capacity=" << v1.capacity() << '\n';
    std::system("pause");
}