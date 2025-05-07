#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "data_struct.hpp"

int main()
{
    using Trukhanov::DataStruct;

    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        DataStruct ds;

        if (iss >> ds)
        {
            data.push_back(ds);
        }
        iss.clear();
    }

    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
        });

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}

