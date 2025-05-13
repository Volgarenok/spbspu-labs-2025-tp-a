#include "test.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

namespace amine {

std::istream &operator>>(std::istream &in, DelimiterIO &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c;
    if (in >> c && c != dest.exp)
        in.setstate(std::ios::failbit);
    return in;
}

std::istream &operator>>(std::istream &in, DoubleSCI &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    double num;
    if (in >> num)
        dest.ref = num;
    else
        in.setstate(std::ios::failbit);
    return in;
}

std::istream &operator>>(std::istream &in, RationalIO &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    long long n;
    unsigned long long d;
    
    in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'};
    in >> n;
    in >> DelimiterIO{':'} >> DelimiterIO{'D'};
    in >> d;
    in >> DelimiterIO{':'} >> DelimiterIO{')'};

    if (d == 0 || !in)
        in.setstate(std::ios::failbit);
    else
        dest.ref = {n, d};
    return in;
}

std::istream &operator>>(std::istream &in, StringIO &&dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char quote;
    if (in >> quote && quote != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string str;
    char c;
    while (in.get(c) {
        if (c == '\\') {
            if (!in.get(c)) break;
            str += c;
        }
        else if (c == '"') {
            break;
        }
        else {
            str += c;
        }
    }

    if (c != '"')
        in.setstate(std::ios::failbit);
    else
        dest.ref = str;
    return in;
}

}
