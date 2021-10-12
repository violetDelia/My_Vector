#include <my_vector.hpp>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <new>
#include <numeric>
int main()
{
    my_vector::vector<char> characters;

    characters.assign(5, 'a');

    for (char c : characters)
    {
        std::cout << c << ' ';
    }

    characters.assign({'\n', 'C', '+', '+', '1', '1', '\n'});

    for (char c : characters)
    {
        std::cout << c;
    }
    std::system("pause");
}
