#include "polygon.hpp"
#include <cmath>
#include <functional>
#include <algorithm>
#include <iterator>
#include <numeric>
#include "wrappers_io.hpp"

std::istream& nehvedovich::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point temp{ 0, 0 };
  in >> DelimiterIO{ '(' } >> temp.x >> DelimiterIO{ ';' } >> temp.y >> DelimiterIO{ ')' };

  if (in)
  {
    point = temp;
  }
  return in;
}

std::istream& nehvedovich::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t count;
  in >> count;
  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > temp;
  using input_it_t = std::istream_iterator< Point >;
  std::copy_n(input_it_t{ in }, count, std::back_inserter(temp));

  if (in)
  {
    polygon.points = temp;
  }

  return in;
}

bool nehvedovich::operator==(const Point& a, const Point& b)
{
  return a.x == b.x && a.y == b.y;
}

bool nehvedovich::operator==(const Polygon& p1, const Polygon& p2)
{
  return p1.points == p2.points;
}

bool nehvedovich::operator!=(const Polygon& p1, const Polygon& p2)
{
  return !(p1 == p2);
}

bool nehvedovich::isEven(const Polygon& p)
{
  return p.points.size() % 2 == 0;
}

bool nehvedovich::isOdd(const Polygon& p)
{
  return p.points.size() % 2 == 1;
}

bool nehvedovich::hasVertexCount(const Polygon& p, size_t count)
{
  return p.points.size() == count;
}

bool nehvedovich::compareByVertices(const nehvedovich::Polygon& p1, const nehvedovich::Polygon& p2)
{
  return p1.points.size() < p2.points.size();
}

bool nehvedovich::compareByArea(const Polygon& p1, const Polygon& p2)
{
  return getArea(p1) < getArea(p2);
}

bool nehvedovich::comparePointByX(const Point& a, const Point& b)
{
  return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool nehvedovich::comparePointByY(const Point& a, const Point& b)
{
  return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}

double nehvedovich::getDistance(const Point& point1, const Point& point2)
{
  return (std::sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y)));
}

double nehvedovich::getTriangleArea(const Point& point1, const Point& point2, const Point& point3)
{
  double a = getDistance(point1, point2);
  double b = getDistance(point2, point3);
  double c = getDistance(point1, point3);
  double p = (a + b + c) / 2.0;
  return (std::sqrt(p * (p - a) * (p - b) * (p - c)));
}

double nehvedovich::getArea(const Polygon& polygon)
{
  std::vector< double > areas(polygon.points.size() - 2);
  const auto& begin = polygon.points.cbegin();
  const auto& end = polygon.points.cend();
  using namespace std::placeholders;
  auto triangleAreaCalc = std::bind(getTriangleArea, polygon.points[0], _1, _2);
  std::transform(begin + 1, end - 1, begin + 2, std::back_inserter(areas), triangleAreaCalc);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}
