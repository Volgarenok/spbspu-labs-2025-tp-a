#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data.hpp"
int main()
{
    using DataVector = std::vector<amine::DataStruct>;
    constexpr auto max_ignore = std::numeric_limits<std::streamsize>::max();
    DataVector data;
    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator<amine::DataStruct>(std::cin),
            std::istream_iterator<amine::DataStruct>(),
            std::back_inserter(data)
        );
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(max_ignore, '\n');
        }
    }
    std::sort(data.begin(), data.end());
    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<amine::DataStruct>(std::cout, "\n")
    );
    return 0;
}
