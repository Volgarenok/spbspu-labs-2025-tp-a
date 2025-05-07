#include "DataStruct.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<nspace::DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        nspace::DataStruct entry;
        if (iss >> entry) {
            data.push_back(entry);
        }
    }

    std::sort(data.begin(), data.end(), [](const nspace::DataStruct& a, const nspace::DataStruct& b) {
        if (a.key1 != b.key1)
            return a.key1 < b.key1;
        long double valueA = static_cast<long double>(a.key2.first) / a.key2.second;
        long double valueB = static_cast<long double>(b.key2.first) / b.key2.second;
        if (valueA != valueB)
            return valueA < valueB;
        return a.key3.length() < b.key3.length();
    });

    for (const nspace::DataStruct& item : data) {
        std::cout << item << '\n';
    }

    return 0;
}
