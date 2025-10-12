#include "Wrapper.hpp"

#include <iostream>

namespace {

    bool isOctalDigit(char c)
    {
        return c >= '0' && c <= '7';
    }

    bool isExpectedULLSequence(char a, char b, char c)
    {
        return a == 'u' && b == 'l' && c == 'l';
    }

} //anonymous namespace

std::istream& milchevskiy::operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char c = 0;
    in >> c;
    if (in && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, LITvalue&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    unsigned long long num = 0;
    char temp = 0;

    while (in >> temp && std::isdigit(temp))
    {
        num = num * 10 + (temp - '0');
    }

    char a = temp, b = 0, c = 0;
    if (in && isExpectedULLSequence(a, b, c))
    {
        in >> b >> c;
        if (in && isExpectedULLSequence(a, b, c))
        {
            dest.ref = num;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, OCTvalue&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char prefix = 0;
    in >> prefix;
   
    if (!in || prefix != '0')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    unsigned long long num = 0;
    char temp = 0;

    while (in >> temp && isOctalDigit(temp))
    {
        num = num * 8 + (temp - '0');
    }

    if (in && !isOctalDigit(temp))
    {
        in.putback(temp);
    }

    dest.ref = num;
    return in;
}

std::istream& milchevskiy::operator>>(std::istream& in, STRvalue&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
