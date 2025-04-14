#include "Wrapper.hpp"

#include <iostream>

std::istream& milchevskiy::operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char c = 0;
    if ((in >> c) && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, UnsignedLongLongOCT&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char prefix = 0;
    if (!(in >> prefix) || prefix != '0')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    unsigned long long num = 0;
    char temp = 0;
    bool gotDigit = false;

    while (in.get(temp) && temp >= '0' && temp <= '7')
    {
        gotDigit = true;
        num = num * 8 + (temp - '0');
    }
    in.unget();

    if (!gotDigit)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref = num;
    return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, UnsignedLongLongULL&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    unsigned long long num = 0;
    char a = 0, b = 0, c = 0;
    char temp = 0;
    bool gotdigit = false;

    while (in >> temp && std::isdigit(temp))
    {
        gotdigit = true;
        num *= 10;
        num += (temp - '0');
    }
    in.unget();

    if (!gotdigit)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (in >> a >> b >> c && tolower(a) == 'u' && tolower(b) == 'l' && tolower(c) == 'l')
    {
        dest.ref = num;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
};