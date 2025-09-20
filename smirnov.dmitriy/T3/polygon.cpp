#include "polygon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include "delimiters.hpp"

struct PolygonArea
{
  smirnov::Point first;
  double operator()(const smirnov::Point& second)
  {
    double area = (second.x + first.x) * (first.y - second.y);
    first = second;
    return area;
  }
};

std::istream& smirnov::operator>>(std::istream& in, Point& value)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  using del = smirnov::DelimiterChar;
  int x = 0;
  int y = 0;
  in >> del{ '(' } >> x >> del{ ';' } >> y >> del{ ')' };
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
  if (p1.x != p2.x)
  {
    return p1.x < p2.x;
  }
  return p1.y < p2.y;
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
  using inputItT = std::istream_iterator< smirnov::Point >;
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
  else
  {
    value = Polygon{ vec };
  }
  return in;
}

bool smirnov::operator==(const Polygon& p1, const Polygon& p2)
{
  return p1.points == p2.points;
}

bool smirnov::operator<=(const Point& first, const Point& second)
{
  return !(second < first);
}

bool smirnov::operator>=(const Point& first, const Point& second)
{
  return !(first < second);
}

double smirnov::getArea(const Polygon& p)
{
  std::vector< double > partArea;
  std::transform(p.points.begin(), p.points.end(), std::back_inserter(partArea), PolygonArea{ p.points[0] });
  double area = std::accumulate(partArea.cbegin(), partArea.cend(), 0.0);
  area += (p.points[p.points.size() - 1].x + p.points[0].x) * (p.points[p.points.size() - 1].y - p.points[0].y);
  return std::abs(area) / 2;
}

bool smirnov::isPerpendicular(const Point& p1, const Point& p2, const Point& p3)
{
  double x1 = p1.x - p2.x;
  double y1 = p1.y - p2.y;
  double x2 = p1.x - p3.x;
  double y2 = p1.y - p3.y;
  return x1 * x2 + y1 * y2 == 0;
}

bool smirnov::isRect(const Polygon& p)
{
  if (p.points.size() != 4)
  {
    return false;
  }
  std::vector< Point > vec(p.points);
  std::sort(vec.begin(), vec.end());
  return isPerpendicular(vec[0], vec[1], vec[2]) && isPerpendicular(vec[1], vec[0], vec[3]) &&
         isPerpendicular(vec[3], vec[2], vec[1]);
}

bool smirnov::isOddCountVertexes(const Polygon& p)
{
  return p.points.size() % 2;
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
