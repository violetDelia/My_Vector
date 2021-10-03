#include "my_vector.hpp"
#include <iostream>
#include <memory>
#include <new>
#include <vector>
void test()
{
    my_vector::vector_allocator<int> alloc;
    my_vector::vector<int> a(12, 5, alloc);
    a.print_member_info();
    a.print_capacity();
    a.reserve(24);
    a.print_member_info();
    a.print_capacity();
    my_vector::vector_const_iterator<my_vector::vector<int>> it;
    auto test = it;
}
int main()
{
    test();
    std::system("pause");
    return 0;
}
