#include "FormatWrapper.hpp"
#include "Delimiter.hpp"
#include "FormatGuard.hpp"
#include <iomanip>
//(:key1 10:key2 3.5:key3 "Hello":)
std::istream& operator>>(std::istream& in, DataStruct& value)
{
    DataStruct tmp;
    {
        FormatGuard guard(in);
        in >> DelimiterIstream("(");
        bool gotKey1 = false;
        bool gotKey2 = false;
        bool gotKey3 = false;
        std::string key;
        while (in && !(gotKey1 && gotKey2 && gotKey3))
        {
            in >> DelimiterIstream(":");
            in >> key;
            if (key == "key1")
            {
                in >> tmp.key1_;
                gotKey1 = true;
            }
            else if (key == "key2")
            {
                in >> tmp.key2_;
                gotKey2 = true;
            }
            else if (key == "key3")
            {
                in >> std::quoted(tmp.key3_);
                gotKey3 = true;
            }
            else
            {
                in.setstate(std::ios::failbit);
            }
        }
        in >> DelimiterIstream(":)");
    }
    if (in)
    {
        value = tmp;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& value)
{
    FormatGuard guard(out);
    out << "(:key1 " << value.key1_
        << ":key2 " << value.key2_
        << ":key3 " << std::quoted(value.key3_)
        << ":)";
    return out;
}
