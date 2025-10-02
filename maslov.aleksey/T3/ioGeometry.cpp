#include "ioGeometry.hpp"
#include <ioDelimiter.hpp>
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <numeric>

namespace
{
  int scalarProduct(const maslov::Point & a,
      const maslov::Point & b, const maslov::Point & c)
  {
    int vectorX1 = b.x - a.x;
    int vectorY1 = b.y - a.y;
    int vectorX2 = c.x - b.x;
    int vectorY2 = c.y - b.y;
    return vectorX1 * vectorX2 + vectorY1 * vectorY2;
  }

  double vectorProduct(const maslov::Point & p0,
      const maslov::Point & p1, const maslov::Point & p2)
  {
    double d1 = (p1.x - p0.x) * (p2.y - p0.y);
    double d2 = (p2.x - p0.x) * (p1.y - p0.y);
    return 0.5 * std::abs(d1 - d2);
  }
}

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
  dest.points.clear();
  dest.points.reserve(num);
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

bool maslov::isEven(const Polygon & polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool maslov::isOdd(const Polygon & polygon)
{
  return !isEven(polygon);
}

bool maslov::isRectangle(const Polygon & polygon)
{
  if (polygon.points.size() != 4)
  {
    return false;
  }
  const auto & p = polygon.points;
  bool check = (scalarProduct(p[0], p[1], p[2]) == 0) && (scalarProduct(p[1], p[2], p[3]) == 0);
  return check && (scalarProduct(p[2], p[3], p[0]) == 0);
}

bool maslov::hasNVertexes(const Polygon & polygon, size_t num)
{
  return polygon.points.size() == num;
}

double maslov::getPolygonArea(const Polygon & polygon)
{
  std::vector< double > areas(polygon.points.size());
  const auto & begin = polygon.points.begin();
  const auto & end = polygon.points.end();
  using namespace std::placeholders;
  auto calc = std::bind(vectorProduct, polygon.points[0], _1, _2);
  std::transform(begin + 1, end - 1, begin + 2, std::back_inserter(areas), calc);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool maslov::compareVertexes(const Polygon & poly1, const Polygon & poly2)
{
  return poly1.points.size() < poly2.points.size();
}
