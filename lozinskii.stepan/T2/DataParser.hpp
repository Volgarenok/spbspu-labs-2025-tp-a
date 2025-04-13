#pragma once
#include <vector>
#include <iostream>
#include "DataStruct.hpp"

class DataParser {
public:
    static std::vector<DataStruct> parseFromStream(std::istream& in);
};