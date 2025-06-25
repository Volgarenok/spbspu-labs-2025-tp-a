#include "geometry.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <stream-guard.hpp>

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    if (!(in >> c) || c != rhs.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

std::istream& aleksandrov::operator>>(std::istream& in, Point& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using sep = DelimiterIO;
  return in >> sep{ '(' } >> rhs.x >> sep{ ';' } >> rhs.y >> sep{ ')' };
}

std::istream& aleksandrov::operator>>(std::istream& in, Polygon& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t verticesCount = 0;
  if (!(in >> verticesCount) || verticesCount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(verticesCount);
  std::copy_n(std::istream_iterator< Point >(in), verticesCount, points.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  rhs.points = points;
  return in;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Point& rhs)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  return out << '(' << rhs.x << ';' << rhs.y << ')';
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Polygon& rhs)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  out << rhs.points.size();
  std::copy(rhs.points.begin(), rhs.points.end(), std::ostream_iterator< Point >(out, " "));
  return out;
}

