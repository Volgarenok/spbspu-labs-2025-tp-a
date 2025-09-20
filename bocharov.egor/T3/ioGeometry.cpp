#include "ioGeometry.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <numeric>
#include <ioDelimiter.hpp>

namespace
{
  double vectorProduct(const bocharov::Point & p0, const bocharov::Point & p1, const bocharov::Point & p2)
  {
    double d1 = (p1.x - p0.x) * (p2.y - p0.y);
    double d2 = (p2.x - p0.x) * (p1.y - p0.y);
    return 0.5 * std::abs(d1 - d2);
  }
}

int bocharov::scalarProduct(const Point & a, const Point & b, const Point & c)
{
  int vectorX1 = b.x - a.x;
  int vectorY1 = b.y - a.y;
  int vectorX2 = c.x - b.x;
  int vectorY2 = c.y - b.y;
  return vectorX1 * vectorX2 + vectorY1 * vectorY2;
}

std::istream & bocharov::operator>>(std::istream & in, Point & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> dest.x >> DelimiterIO{';'} >> dest.y >> DelimiterIO{')'};
  return in;
}

std::istream & bocharov::operator>>(std::istream & in, Polygon & dest)
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
  dest.points.clear();
  using iIterator = std::istream_iterator< Point >;
  std::copy_n(iIterator(in), num - 1, std::back_inserter(dest.points));
  if (in.peek() != '\n')
  {
    std::copy_n(iIterator(in), 1, std::back_inserter(dest.points));
  }
  if (!in || in.peek() != '\n' || dest.points.size() != num)
  {
    in.setstate(std::ios::failbit);
    dest.points.clear();
  }
  return in;
}

bool bocharov::Point::operator==(const Point & rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool bocharov::Polygon::operator==(const Polygon & rhs) const
{
  return points == rhs.points;
}

bool bocharov::isEven(const Polygon & polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool bocharov::isOdd(const Polygon & polygon)
{
  return !isEven(polygon);
}

bool bocharov::hasNVertexes(const Polygon & polygon, size_t num)
{
  return polygon.points.size() == num;
}

double bocharov::getPolygonArea(const Polygon & polygon)
{
  std::vector< double > areas(polygon.points.size());
  const auto & begin = polygon.points.begin();
  const auto & end = polygon.points.end();
  using namespace std::placeholders;
  auto calc = std::bind(vectorProduct, polygon.points[0], _1, _2);
  std::transform(begin + 1, end - 1, begin + 2, std::back_inserter(areas), calc);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool bocharov::compareVertexes(const Polygon & poly1, const Polygon & poly2)
{
  return poly1.points.size() < poly2.points.size();
}
