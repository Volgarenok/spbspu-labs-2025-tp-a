#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include <utility>

namespace beshimow {
    struct DataStruct {
        double key1;
        std::pair<long long, long long> key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& dst);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dst);
    bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
