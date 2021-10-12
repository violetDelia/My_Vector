#include "my_vector.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    my_vector::vector<std::string> letters;

    letters.push_back("abc");
    std::string s = "def";
    letters.push_back(std::move(s));

    std::cout << "vector holds: ";
    for (auto &&i : letters)
        std::cout << std::quoted(i) << ' ';
    std::cout << "\nMoved-from string holds " << std::quoted(s) << '\n';
    std::system("pause");
}