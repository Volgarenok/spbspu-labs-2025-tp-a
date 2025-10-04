#include "polygon.hpp"
#include "delimiter.hpp"
#include <iterator>
#include <algorithm>
#include <cctype>

std::istream & guseynov::operator>>(std::istream & in, Point & point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::streampos start = in.tellg();
  in >> DelimiterI{'('} >> point.x >> DelimiterI{';'} >> point.y >> DelimiterI{')'};
  if (in)
  {
    std::streampos end = in.tellg();
    if (end - start < 5)
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::ostream & guseynov::operator<<(std::ostream & out, const Point & point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

bool guseynov::operator==(const Point & lhs, const Point & rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream & guseynov::operator>>(std::istream & in, Polygon & polygon)
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

bool guseynov::operator==(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points == rhs.points;
}
