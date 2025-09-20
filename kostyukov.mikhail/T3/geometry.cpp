#include "geometry.hpp"

#include <algorithm>
#include <istream>
#include <iterator>
#include <numeric>
#include <vector>

#include "delimiter.hpp"

std::istream& kostyukov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point temp;
  {
    using del = DelimiterIO;
    in >> del{ '(' } >> temp.x >> del{ ';' } >> temp.y >> del{ ')' };
    if (in)
    {
      point = temp;
    }
  }
  return in;
}

bool kostyukov::operator==(const Point& point1, const Point& point2)
{
  return (point1.x == point2.x && point1.y == point2.y) || (point1.x == point2.y && point1.y == point2.x);
}

std::istream& kostyukov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (in)
  {
    polygon.points = std::move(temp);
  }
  return in;
}

namespace
{
  struct CrossProduct
  {
    double operator()(const kostyukov::Point& point1, const kostyukov::Point& point2) const
    {
      return point1.x * point2.y - point2.x * point1.y;
    }
  };
}

double kostyukov::getArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }

  auto begin = polygon.points.begin();
  auto end = polygon.points.end();
  double result = std::inner_product(begin, end - 1, begin + 1, 0.0, std::plus< double >(), CrossProduct{});
  result += CrossProduct{}(polygon.points.back(), polygon.points.front());
  return std::abs(result) / 2.0;
}
