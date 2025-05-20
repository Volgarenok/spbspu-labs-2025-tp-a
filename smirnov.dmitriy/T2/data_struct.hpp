#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>

namespace smirnov {
    struct DataStruct {
        double key1;
        unsigned long long key2;
        std::string key3;
        std::string raw_bin;
    };

    bool compareDataStruct(const DataStruct& a, const DataStruct& b);
    std::istream& operator>>(std::istream& is, DataStruct& ds);
    std::ostream& operator<<(std::ostream& os, const DataStruct& ds);
}

#endif
