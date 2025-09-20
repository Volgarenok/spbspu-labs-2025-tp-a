#include <algorithm>
#include <iterator>
#include "io-utils.h"

std::istream& sherkunov::operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& sherkunov::operator>>(std::istream& in, Point& point)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    return in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
}

std::istream& sherkunov::operator>>(std::istream& in, Polygon& polygon)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    size_t vertexesCount = 0;
    if (!(in >> vertexesCount) || vertexesCount < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::vector< Point > points(vertexesCount);
    std::copy_n(std::istream_iterator< Point >(in), vertexesCount, points.begin());
    if (!in)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    polygon.points = points;
    return in;
}

std::ostream& sherkunov::operator<<(std::ostream& out, const Point& point)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    IoGuard guard(out);
    return out << '(' << point.x << ';' << point.y << ')';
}

std::ostream& sherkunov::operator<<(std::ostream& out, const Polygon& polygon)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    IoGuard guard(out);
    out << polygon.points.size();
    for (size_t i = 0; i < polygon.points.size(); ++i)
    {
        out << ' ' << polygon.points[i];
    }
    return out;
}

sherkunov::IoGuard::IoGuard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{
}

sherkunov::IoGuard::~IoGuard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}
