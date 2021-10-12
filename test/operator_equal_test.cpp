#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
#include <vector>

void display_sizes(char const *comment,
                   const my_vector ::vector<std::string> &nums1,
                   const my_vector ::vector<std::string> &nums2,
                   const my_vector ::vector<std::string> &nums3)
{
    std::cout << comment
              << " nums1: " << nums1.size() << ','
              << " nums2: " << nums2.size() << ','
              << " nums3: " << nums3.size() << '\n';
}

void display(char const *comment, const my_vector ::vector<std::string> &v)
{
    std::cout << comment << "{ ";
    for (auto e : v)
    {
        std::cout << e << ' ';
    }
    std::cout << "}\n";
}

void test()
{
    my_vector ::vector<std::string> nums1{"3", "1", "4", "6", "5", "9"};
    my_vector ::vector<std::string> nums2;
    my_vector ::vector<std::string> nums3;


    display_sizes("Initially:\n", nums1, nums2, nums3);

    // 从 nums1 复制赋值数据到 nums2
    nums2 = nums1;

    display_sizes("After assigment:\n", nums1, nums2, nums3);

    // 从 nums1 移动赋值数据到 nums3,
    // 修改 nums1 和 nums3
    nums3 = std::move(nums1);

    display_sizes("After move assigment:\n", nums1, nums2, nums3);

    display("Now nums3 = ", nums3);

     //initializer_list 的复制赋值复制数据给 nums3
    nums3 = {"1", "2", "3"};

    display("After assignment of initializer_list \n nums3 = ", nums3);
    display("\n nums2 = ", nums2);
    display(" \nnums1 = ", nums1);

}
void log_set();
int main()
{
    log_set();
    test();
    std::system("pause");
    return 0;
}
void log_set()
{
    my_vector::show_allocate_and_deallocate_log = false;
    my_vector::show_destory_log = false;
    ;
    my_vector::show_construct_log = false;
}
