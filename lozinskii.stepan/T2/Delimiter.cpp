#include "Delimiter.hpp"

DelimiterIstream::DelimiterIstream(const std::string& expected) :
    expected_(expected)
{
}

std::istream& operator>>(std::istream& in, const DelimiterIstream& delim)
{
    for (char c : delim.expected_)
    {
        char got = 0;
        in >> got;
        if (!in || got != c)
        {
            in.setstate(std::ios::failbit);
            break;
        }
    }
    return in;
}
