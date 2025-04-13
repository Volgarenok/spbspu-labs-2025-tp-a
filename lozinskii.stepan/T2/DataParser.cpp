#include "DataParser.hpp"
#include <algorithm>
#include <iterator>

std::vector<DataStruct> DataParser::parseFromStream(std::istream& in) {
    std::vector<DataStruct> data;
    std::copy(
        std::istream_iterator<DataStruct>(in),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );
    return data;
}