#include "DataStruct.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

int main() {
    std::vector<DataStruct> data;

    std::istream_iterator<DataStruct> inputBegin(std::cin);
    std::istream_iterator<DataStruct> inputEnd;
    std::copy(inputBegin, inputEnd, std::back_inserter(data));

    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1)
            return a.key1 < b.key1;
        if (a.key2 != b.key2)
            return (static_cast<long double>(a.key2.first) / a.key2.second) <
                   (static_cast<long double>(b.key2.first) / b.key2.second);
        return a.key3.length() < b.key3.length();
    });

    std::ostream_iterator<DataStruct> output(std::cout, "\n");
    std::copy(data.begin(), data.end(), output);

    return 0;
}
