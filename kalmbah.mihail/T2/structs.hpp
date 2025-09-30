#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
#include <utility>
#include <iosfwd>

namespace kalmbah {

struct DataStruct {
    unsigned long long key1_;
    std::pair<long long, unsigned long long> key2_;
    std::string key3_;
};

struct Delimiter {
    char expected;
};

struct UllOct {
    unsigned long long& value;
};

struct RatLsp {
    std::pair<long long, unsigned long long>& value;
};

struct StringToken {
    std::string& value;
};

std::istream& operator>>(std::istream& in, Delimiter&& delim);
std::istream& operator>>(std::istream& in, UllOct&& ullOct);
std::istream& operator>>(std::istream& in, RatLsp&& rat);
std::istream& operator>>(std::istream& in, StringToken&& strToken);
std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
bool operator<(const DataStruct& left, const DataStruct& right);

}

#endif
