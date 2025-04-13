#pragma once
#include <iostream>
#include <string>

struct DataStruct {
    long long key1;
    double key2;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& ds);
std::ostream& operator<<(std::ostream& out, const DataStruct& ds);