#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>

namespace milchevskiy
{
    struct DataStruct
    {
        unsigned long long key1;
        unsigned long long key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
    bool operator<(const DataStruct& a, const DataStruct& b);
}

#endif