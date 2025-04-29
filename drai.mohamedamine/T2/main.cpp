#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "DataStruct.h"

int main()
{
    using nspace::DataStruct;
    std::vector<DataStruct> data;

    // Read from stdin (or file)
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    // Sort
    std::sort(data.begin(), data.end(), nspace::compare);

    // Output
    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}