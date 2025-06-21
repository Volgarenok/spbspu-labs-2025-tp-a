#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <delimiter.hpp>

std::istream& shak::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point point{0, 0};
  in >> DelimiterIO{'('} >> point.x >> DelimiterIO{';'} >> point.y >> DelimiterIO{')'};
  if (in)
  {
    dest = point;
  }
  return in;
}

bool shak::operator==(const Point &point1, const Point &point2)
{
  return ((point1.x == point2.x) && (point1.y == point2.y));
}

bool shak::operator<(const Point &point1, const Point &point2)
{
  return ((point1.x < point2.x) && (point1.y < point2.y));
}

bool shak::operator<=(const Point &point1, const Point &point2)
{
  return ((point1.x <= point2.x) && (point1.y <= point2.y));
}

bool shak::operator>=(const Point &point1, const Point &point2)
{
  return ((point1.x >= point2.x) && (point1.y >= point2.y));
}

double shak::getDistance(const Point &point1, const Point &point2)
{
  return (sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y)));
}

std::istream& shak::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  size_t pointsCount = 0;
  in >> pointsCount;
  if (pointsCount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > polygon;
  std::copy_n(std::istream_iterator< Point >{in}, pointsCount, std::back_inserter(polygon));

  if (in && polygon.size() == pointsCount)
  {
    dest.points = polygon;
  }
  return in;
}

bool shak::isEven(const Polygon &polygons)
{
  return (polygons.points.size() % 2 == 0);
}

bool shak::isOdd(const Polygon &polygons)
{
  return (polygons.points.size() % 2 != 0);
}

bool shak::areEqualVertexes(size_t numOfVertexes, const Polygon& polygon)
{
  return (polygon.points.size() == numOfVertexes);
}

bool shak::checkRectangle(const Polygon &polygon)
{
  if (polygon.points.size() != 4)
  {
    return false;
  }
  const auto &points = polygon.points;
  const double epsilon = 1e-9;
  double side1 = getDistance(points[0], points[1]);
  double side2 = getDistance(points[1], points[2]);
  double side3 = getDistance(points[2], points[3]);
  double side4 = getDistance(points[3], points[0]);
  bool isOpositeEqual = std::abs(side1 - side3) < epsilon && std::abs(side2 - side4) < epsilon;
  if (!isOpositeEqual) {
    return false;
  }
  double diagonal1 = getDistance(points[0], points[2]);
  double diagonal2 = getDistance(points[1], points[3]);
  return std::abs(diagonal1 - diagonal2) < epsilon;
}

double shak::getArea(const Polygon &polygon)
{
  TriangleArea function{polygon.points[0], polygon.points[1]};
  std::vector< double > areas(polygon.points.size());
  std::transform(std::begin(polygon.points) + 2, std::end(polygon.points), areas.begin(), function);
  return std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >{});
}

size_t shak::getVertexes(const Polygon &polygon)
{
  return polygon.points.size();
}

size_t shak::equalCounter(const Polygon &polygon, const std::vector< Point > &targetPoints, size_t &counter)
{
  if (targetPoints == polygon.points)
  {
    counter++;
  }
  else
  {
    counter = 0;
  }
  return counter;
}

double shak::TriangleArea::operator()(const Point & point3)
{
  double a = 0.0;
  double b = 0.0;
  double c = 0.0;
  double p = 0.0;
  a = getDistance(point1, point2);
  b = getDistance(point2, point3);
  c = getDistance(point1, point3);
  p = (a + b + c) / 2;
  point2 = point3;
  return (sqrt(p * (p - a) * (p - b) * (p - c)));
}
