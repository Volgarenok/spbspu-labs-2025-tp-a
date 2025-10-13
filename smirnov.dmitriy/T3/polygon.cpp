#include "polygon.hpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <vector>
#include <limits>
#include "delimiters.hpp"

std::istream& smirnov::operator>>(std::istream& in, Point& value)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  int x{ 0 };
  int y{ 0 };
  in >> DelimiterChar{ '(' } >> x >> DelimiterChar{ ';' } >> y >> DelimiterChar{ ')' };
  if (in)
  {
    value = Point{ x, y };
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool smirnov::operator<(const Point& p1, const Point& p2)
{
  return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

bool smirnov::operator==(const Point& p1, const Point& p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

std::istream& smirnov::operator>>(std::istream& in, Polygon& value)
{
  std::istream::sentry guard(in);
  if (!guard)
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
  using inputItT = std::istream_iterator< Point >;
  std::vector< Point > vec;
  vec.reserve(n);

  std::copy_n(inputItT{ in }, n - 1, std::back_inserter(vec));
  if (in.peek() != '\n')
  {
    std::copy_n(inputItT{ in }, 1, std::back_inserter(vec));
  }
  if (!in || vec.size() != n || in.peek() != '\n')
  {
    in.setstate(std::ios::failbit);
  }
  value = Polygon{ std::move(vec) };
  return in;
}

bool smirnov::operator==(const Polygon& p1, const Polygon& p2)
{
  return p1.points == p2.points;
}

bool smirnov::operator<=(const Point& p1, const Point& p2)
{
  return !(p2 < p1);
}

bool smirnov::operator>=(const Point& p1, const Point& p2)
{
  return !(p1 < p2);
}

struct PolygonArea
{
  smirnov::Point previousPoint;
  double operator()(double currentSum, const smirnov::Point& currentPoint)
  {
    double segmentArea = (currentPoint.x + previousPoint.x) * (previousPoint.y - currentPoint.y);
    previousPoint = currentPoint;
    return currentSum + segmentArea;
  }
};

double smirnov::getArea(const Polygon& p)
{
  if (p.points.empty())
  {
    return 0.0;
  }

  PolygonArea areaCalc{p.points[0]};

  double area = std::accumulate(p.points.begin() + 1, p.points.end(),0.0,std::ref(areaCalc));

  area += (p.points.back().x + p.points.front().x) * (p.points.back().y - p.points.front().y);

  return std::abs(area) / 2.0;
}

bool smirnov::isPerpendicular(const Point& p1, const Point& p2, const Point& p3)
{
  double x1 = p1.x - p2.x;
  double y1 = p1.y - p2.y;
  double x2 = p1.x - p3.x;
  double y2 = p1.y - p3.y;
  return std::abs(x1 * x2 + y1 * y2) < std::numeric_limits< double >::epsilon();
}

bool smirnov::isRect(const Polygon& p)
{
  if (p.points.size() != 4)
  {
    return false;
  }
  std::array< Point, 4 > points;
  std::copy_n(p.points.begin(), 4, points.begin());
  std::sort(points.begin(), points.end());
  return isPerpendicular(points[0], points[1], points[2]) && isPerpendicular(points[1], points[0], points[3]) &&
         isPerpendicular(points[3], points[2], points[1]);
}

bool smirnov::isOddCountVertexes(const Polygon& p)
{
  return p.points.size() % 2 != 0;
}

bool smirnov::isEvenCountVertexes(const Polygon& p)
{
  return p.points.size() % 2 == 0;
}

bool smirnov::isNCountVertexes(const Polygon& p, size_t n)
{
  return p.points.size() == n;
}

bool smirnov::minArea(const Polygon& p1, const Polygon& p2)
{
  return getArea(p1) < getArea(p2);
}

bool smirnov::minVertexes(const Polygon& p1, const Polygon& p2)
{
  return p1.points.size() < p2.points.size();
}
