#include "structs.hpp"
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <limits>

int main() {
    using namespace kalmbah;

    std::vector<DataStruct> data;

    while (!std::cin.eof()) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(std::move(temp));
        }
    }

    std::sort(data.begin(), data.end());

    std::copy(
        data.cbegin(),
        data.cend(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
