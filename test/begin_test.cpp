#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>

int main()
{
	my_vector::vector<int> nums {1, 2, 4, 8, 16};
	my_vector::vector<std::string> fruits {"orange", "apple", "raspberry"};
	my_vector::vector<char> empty;
 
	// 求和 vector nums 中的所有整数（若存在），仅打印结果。
	std::cout << "Sum of nums: " <<
                std::accumulate(nums.begin(), nums.end(), 0) << "\n";
 
	// 打印 vector fruits 中的首个 fruis ，不检查是否有一个。
	std::cout << "First fruit: " << *fruits.begin() << "\n";
 
	if (empty.begin() == empty.end())
		std::cout << "vector 'empty' is indeed empty.\n";
        std::system("pause");
}