#include "structs.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

int main()
{
    using namespace kalmbah;

    std::vector< DataStruct > data;

    DataStruct temp;
    while (std::cin >> temp) {
        data.push_back(std::move(temp));
        temp = DataStruct{};
    }

    if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

        while (std::cin >> temp) {
            data.push_back(std::move(temp));
            temp = DataStruct{};
        }
    }

    std::sort(data.begin(), data.end());

    std::copy(
        data.cbegin(),
        data.cend(),
        std::ostream_iterator< DataStruct >(std::cout, "\n")
    );

    return 0;
}
