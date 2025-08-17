#include "data-struct.hpp"
#include "../common/delimiter.hpp"
#include "io-helpers.hpp"
#include "../common/stream-guard.hpp"

bool elagin::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
    if (lhs.key1 != rhs.key1)
    {
        return lhs.key1 < rhs.key1;
    }
    if (std::abs(lhs.key2.real() - rhs.key2.real()) > 1e-6)
    {
        return lhs.key2.real() < rhs.key2.real();
    }
    if (std::abs(lhs.key2.imag() - rhs.key2.imag()) > 1e-6)
    {
        return lhs.key2.imag() < rhs.key2.imag();
    }
    return lhs.key3.size() < rhs.key3.size();
}

std::istream &elagin::operator>>(std::istream &in, DataStruct &dst)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    DataStruct temp;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    in >> DelimiterIO{'('};
    for (int i = 0; i < 3; ++i)
    {
        in >> DelimiterIO{':'} >> DelimiterIO{'k'} >> DelimiterIO{'e'} >> DelimiterIO{'y'};
        int keyNum = 0;
        in >> keyNum;

        if (keyNum == 1 && !hasKey1)
        {
            in >> HexIOIn{temp.key1};
            hasKey1 = true;
        }
        else if (keyNum == 2 && !hasKey2)
        {
            in >> ComplexIOIn{temp.key2};
            hasKey2 = true;
        }
        else if (keyNum == 3 && !hasKey3)
        {
            in >> StringIO{temp.key3};
            hasKey3 = true;
        }
        else
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    in >> DelimiterIO{':'} >> DelimiterIO{')'};

    if (in && hasKey1 && hasKey2 && hasKey3)
    {
        dst = temp;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream &elagin::operator<<(std::ostream &out, const DataStruct &dst)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    StreamGuard guard(out);
    out << "(:key1 " << HexIOOut{dst.key1};
    out << ":key2 " << ComplexIOOut{dst.key2};
    out << ":key3 \"" << dst.key3 << "\":)";

    return out;
}