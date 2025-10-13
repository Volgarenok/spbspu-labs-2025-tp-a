#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "data_struct.hpp"

int main()
{
    using namespace ignashov;

    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    if (!std::cin.eof()) {
        std::cin.clear();
    }

    std::sort(data.begin(), data.end());

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}