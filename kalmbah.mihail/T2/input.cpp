#include <cctype>
#include <iostream>
#include "structs.hpp"
#include "stream_guard.hpp"

namespace kalmbah {

std::istream& operator>>(std::istream& in, Delimiter&& delim)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    char actual = 0;
    in >> actual;
    if (in && std::tolower(actual) != std::tolower(delim.expected)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, UllOct&& ullOct)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    StreamGuard guard(in);
    in >> std::oct >> ullOct.value;
    return in;
}

std::istream& operator>>(std::istream& in, RatLsp&& rat)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    long long numerator = 0;
    unsigned long long denominator = 0;

    in >> Delimiter{'('}
       >> Delimiter{':'}
       >> Delimiter{'N'} >> numerator
       >> Delimiter{':'}
       >> Delimiter{'D'} >> denominator
       >> Delimiter{':'}
       >> Delimiter{')'};

    if (!in || denominator == 0) {
        in.setstate(std::ios::failbit);
        return in;
    }

    rat.value = {numerator, denominator};
    return in;
}

std::istream& operator>>(std::istream& in, StringToken&& strToken)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string temp;
    in >> Delimiter{'"'};
    if (!in) {
        return in;
    }

    char ch;
    while (in.get(ch) && ch != '"') {
        if (ch == '\n') {
            in.setstate(std::ios::failbit);
            return in;
        }
        temp += ch;
    }

    if (ch != '"') {
        in.setstate(std::ios::failbit);
    } else {
        strToken.value = std::move(temp);
    }
    return in;
}

}
