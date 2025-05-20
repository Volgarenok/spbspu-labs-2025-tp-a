#include "ioGeometry.hpp"
#include <ioDelimiter.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>

std::istream & maslov::operator>>(std::istream & in, Point & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> dest.x >> DelimiterIO{';'} >> dest.y >> DelimiterIO{')'};
  return in;
}

std::istream & maslov::operator>>(std::istream & in, Polygon & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t num = 0;
  if (!(in >> num) || num < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(num);
  using iIterator = std::istream_iterator< Point >;
  std::copy_n(iIterator(in), num, temp.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.points = std::move(temp);
  return in;
}

std::ostream & maslov::operator<<(std::ostream & out, const Point & dest)
{
  return out << '(' << dest.x << ';' << dest.y << ')';
}

std::ostream & maslov::operator<<(std::ostream & out, const Polygon & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  using oIterator = std::ostream_iterator< Point >;
  out << dest.points.size() << ' ';
  std::copy(dest.points.begin(), dest.points.end(), oIterator(out, " "));
  return out;
}

bool maslov::Point::operator==(const Point & rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool maslov::Polygon::operator==(const Polygon & rhs) const
{
  return points == rhs.points;
}
