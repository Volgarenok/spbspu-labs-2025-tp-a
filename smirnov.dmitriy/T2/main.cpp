#include "data_struct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>

int main()
{
    using  smirnov::Data;

    std::vector<Data> data;

    std::copy(
        std::istream_iterator<Data>(std::cin),
        std::istream_iterator<Data>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), [](const Data& a, const Data& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
        });

    std::copy(data.begin(), data.end(), std::ostream_iterator<Data>(std::cout, "\n"));
    return 0;
}

