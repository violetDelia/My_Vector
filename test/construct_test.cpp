#include "my_vector.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
template <typename T>
std::ostream &operator<<(std::ostream &s, const my_vector::vector<T> &v)
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto &e : v)
    {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

int main()
{
    // C++11 初始化器列表语法：
    my_vector::vector<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';

    // words2 == words1
    my_vector::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';

    // words3 == words1
    my_vector::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';

    // words4 为 {"Mo", "Mo", "Mo", "Mo", "Mo"}
    my_vector::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
    std::system("pause");
}