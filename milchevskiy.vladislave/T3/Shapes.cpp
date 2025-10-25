#include "Shapes.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>

#include "Delimiter.hpp"

bool milchevskiy::operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::istream& milchevskiy::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using del = DelimiterI;
  in >> del{ '(' } >> point.x >> del{ ';' } >> point.y >> del{ ')' };
  return in;
}

bool milchevskiy::operator==(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points == rhs.points;
}

std::istream& milchevskiy::operator>>(std::istream& in, Polygon& pol)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  in >> count;
  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (!in || temp.size() != count)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  pol.points = temp;
  return in;
}
