#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
#include <vector>
void print_vec(const my_vector::vector<int> &vec)
{
    for (auto x : vec)
    {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main()
{
    my_vector::vector<int> vec(3, 100);
    print_vec(vec);

    auto it = vec.begin();
    it = vec.insert(it, 200);
    print_vec(vec);
    it += 1;
    vec.insert(it, 2, 300);
    print_vec(vec);

    // "it" 不再合法，获取新值：
    it = vec.begin();

    my_vector::vector<int> vec2(2, 400);
    vec.insert(it + 2, vec2.begin(), vec2.end());
    print_vec(vec);

    int arr[] = {501, 502, 503};
    vec.insert(vec.begin(), arr, arr + 3);
    print_vec(vec);
    std::system("pause");
}