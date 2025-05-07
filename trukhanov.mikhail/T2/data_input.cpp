#include "data_input.hpp"
#include "stream_guard.hpp"
#include <iomanip>

namespace Trukhanov
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '\0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        for (char expected : dest.exp) {
            char actual = '\0';
            in.get(actual);
            if (!in || actual != expected) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        return in;
    }




    std::istream& operator>>(std::istream& in, OctalULLIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        StreamGuard guard(in);
        in >> std::oct >> dest.ref;
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        long long numerator = 0;
        unsigned long long denominator = 0;

        in >> DelimiterIO{ '(' }
            >> DelimiterIO{ ':' }
            >> LabelIO{ "N" }
            >> numerator
            >> DelimiterIO{ ':' }
            >> LabelIO{ "D" }
            >> denominator
            >> DelimiterIO{ ':' }
        >> DelimiterIO{ ')' };

        if (in && denominator != 0)
        {
            dest.ref = { numerator, denominator };
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }
}
