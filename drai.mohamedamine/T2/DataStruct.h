#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <utility>
#include <iostream>

namespace nspace {

    struct DataStruct {
        double key1;
        std::pair<long long, unsigned long long> key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& data);
    std::ostream& operator<<(std::ostream& out, const DataStruct& data);
    bool isValidDataStruct(const std::string& line);

}

#endif // DATASTRUCT_H
