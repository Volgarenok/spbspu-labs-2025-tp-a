#include "io-utils.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <stream-guard.hpp>

namespace aleksandrov
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '\0';
    if (!(in >> c) || c != rhs.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Point& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using sep = DelimiterIO;
    return in >> sep{ '(' } >> rhs.x >> sep{ ';' } >> rhs.y >> sep{ ')' };
  }

  std::istream& operator>>(std::istream& in, Polygon& rhs)
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
    rhs.points = points;
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Point& rhs)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    return out << '(' << rhs.x << ';' << rhs.y << ')';
  }

  std::ostream& operator<<(std::ostream& out, const Polygon& rhs)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    out << rhs.points.size();
    for (size_t i = 0; i < rhs.points.size(); ++i)
    {
      out << ' ' << rhs.points[i];
    }
    return out;
  }
}

