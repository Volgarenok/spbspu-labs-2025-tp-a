#include "polygon.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <delimetr.hpp>

std::istream & smirnov::operator>>(std::istream & in, Point & point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterI{'('} >> point.x >> DelimiterI{';'} >> point.y >> DelimiterI{')'};
  return in;
}

std::ostream & smirnov::operator<<(std::ostream & out, const Point & point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

bool smirnov::operator==(const Point & lhs, const Point & rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream & smirnov::operator>>(std::istream & in, Polygon & polygon)
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
  std::vector< Point > points(num);
  using istream_it = std::istream_iterator< Point >;
  std::copy_n(istream_it{in}, num, points.begin());
  if (in)
  {
    polygon.points = std::move(points);
  }
  return in;
}

bool smirnov::operator==(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points == rhs.points;
}
