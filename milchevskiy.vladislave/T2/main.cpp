#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "DataStruct.hpp"

int main()
{
    using milchevskiy::DataStruct;
    using input = std::istream_iterator< DataStruct >;
    using output = std::ostream_iterator< DataStruct >;

    std::vector<DataStruct> data;

    while (!std::cin.eof())
    {
        std::copy(input{std::cin}, input{}, std::back_inserter(data));

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end());
    std::copy(data.begin(), data.end(), output(std::cout, "\n"));

    return 0;
}
