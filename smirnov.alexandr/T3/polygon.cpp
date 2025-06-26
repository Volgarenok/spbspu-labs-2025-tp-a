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
  int x = 0, y = 0;
  in >> smirnov::DelimiterI{'('} >> x >> smirnov::DelimiterI{';'} >> y >> smirnov::DelimiterI{')'};
  if (in)
  {
    point = {x, y};
  }
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
  size_t n = 0;
  in >> n;
  if (n < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > pts(n);
  using input_it = std::istream_iterator< Point >;
  std::copy_n(input_it{in}, n, pts.begin());
  if (in && pts.size() == n)
  {
    polygon.points = std::move(pts);
  }
  return in;
}

bool smirnov::operator==(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points == rhs.points;
}

double smirnov::getArea(const Polygon & polygon)
{
  size_t n = polygon.points.size();
  if (n < 3)
  {
    return 0.0;
  }
  int sum = 0;
  for (size_t i = 0; i < n; ++i)
  {
    const Point & p1 = polygon.points[i];
    const Point & p2 = polygon.points[(i + 1) % n];
    sum += p1.x * p2.y - p2.x * p1.y;
  }
  return std::abs(sum) / 2.0;
}
