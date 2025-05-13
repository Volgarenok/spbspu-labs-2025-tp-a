#include "test.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include <limits>

namespace amine {
std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c;
    if (in >> c && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream& operator>>(std::istream& in, DoubleSCI&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    double num;
    if (in >> num) {
        dest.ref = num;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream& operator>>(std::istream& in, RationalIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    long long numerator = 0;
    unsigned long long denominator = 0;

    in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'};
    in >> numerator;
    in >> DelimiterIO{':'} >> DelimiterIO{'D'};
    in >> denominator;
    in >> DelimiterIO{':'} >> DelimiterIO{')'};

    if (denominator == 0) {
        in.setstate(std::ios::failbit);
    } else if (in) {
        dest.ref = {numerator, denominator};
    }
    return in;
}
std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char quote;
    if (in >> quote && quote != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string str;
    bool escape = false;
    char c;

    while (in.get(c)) {
        if (escape) {
            str += c;
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else if (c == '"') {
            break;
        } else {
            str += c;
        }
    }
    if (c != '"') {
        in.setstate(std::ios::failbit);
    } else {
        dest.ref = str;
    }
    return in;
}
}
