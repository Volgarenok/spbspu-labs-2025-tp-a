#include "dataStruct.hpp"
#include <iomanip>
#include <sstream>
#include <cctype>
#include "streamGuard.hpp"
#include "delimeterIO.hpp"

namespace maslovskiy
{

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    std::ostream::sentry s(out);
    if (!s) return out;
    StreamGuard guard(out);

    out << "(";
    out << ":key1 " << SllLitO{ data.key1 };
    out << ":key2 " << "0x" << std::hex << std::uppercase << data.key2;
    out << ":key3 " << "\"" << data.key3 << "\"";
    out << ":)";

    return out;
}

std::istream& operator>>(std::istream& in, SllLitI&& dest)
{
    std::istream::sentry s(in);
    if (!s) return in;

    std::string token;
    if (!(in >> token)) return in;

    if (token.size() < 3 || (token[token.size()-2] != 'l' && token[token.size()-2] != 'L') ||
        (token[token.size()-1] != 'l' && token[token.size()-1] != 'L'))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    token.resize(token.size()-2);
    std::istringstream ss(token);
    ss >> dest.ref;
    if (!ss) in.setstate(std::ios::failbit);

    return in;
}

std::ostream& operator<<(std::ostream& out, const SllLitO&& dest)
{
    std::ostream::sentry s(out);
    if (!s) return out;
    return out << dest.ref << "ll";
}

std::istream& operator>>(std::istream& in, UllHexIO&& dest)
{
    std::istream::sentry s(in);
    if (!s) return in;

    char c0 = '\0', c1 = '\0';
    if (!(in >> c0) || c0 != '0' || !(in >> c1) || std::tolower(c1) != 'x')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    in >> std::hex >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry s(in);
    if (!s) return in;

    in >> DelimiterIO{ '"' };
    return std::getline(in, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DataStruct& data)
{
    std::istream::sentry s(in);
    if (!s) return in;

    DataStruct input;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
    std::string key;
    constexpr size_t keysNumbers = 3;
    size_t count = 0;
    bool wasKey1 = false, wasKey2 = false, wasKey3 = false;

    while (count < keysNumbers && in)
    {
        in >> key;
        if (key == "key1" && !wasKey1)
        {
            in >> SllLitI{ input.key1 };
            in >> DelimiterIO{ ':' };
            wasKey1 = true;
            ++count;
        }
        else if (key == "key2" && !wasKey2)
        {
            in >> UllHexIO{ input.key2 };
            in >> DelimiterIO{ ':' };
            wasKey2 = true;
            ++count;
        }
        else if (key == "key3" && !wasKey3)
        {
            in >> StringIO{ input.key3 };
            in >> DelimiterIO{ ':' };
            wasKey3 = true;
            ++count;
        }
        else
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    in >> DelimiterIO{ ')' };
    if (in) data = input;
    return in;
}

bool operator<(const DataStruct& lhs, const DataStruct& rhs)
{
    if (lhs.key1 == rhs.key1)
    {
        if (lhs.key2 == rhs.key2)
            return lhs.key3.size() < rhs.key3.size();
        else
            return lhs.key2 < rhs.key2;
    }
    else
        return lhs.key1 < rhs.key1;
}

}
