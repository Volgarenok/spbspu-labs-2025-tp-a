#include "DataParser.hpp"
#include "DataSorter.hpp"
#include <iostream>
#include <iterator>

int main() {
    auto data = DataParser::parseFromStream(std::cin);

    DataSorter::sort(data);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}