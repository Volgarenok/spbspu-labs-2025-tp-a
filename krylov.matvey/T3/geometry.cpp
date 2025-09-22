#include "geometry.hpp"
#include <numeric>
#include <algorithm>
#include <functional>
#include <iterator>
#include <delimeterIO.hpp>

std::istream& krylov::operator>>(std::istream& in, Point& p)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point temp{ 0, 0 };
  in >> DelimiterIO{ '(' };
  in >> temp.x;
  in >> DelimiterIO{ ';' };
  in >> temp.y;
  in >> DelimiterIO{ ')' };
  if (in)
  {
    p = temp;
  }
  return in;
}

std::istream& krylov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t k = 0;
  in >> k;
  if (k < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(k);
  std::copy_n(std::istream_iterator< Point >(in), k, points.begin());
  if (!in)
  {
    return in;
  }
  polygon.points = points;
  return in;
}

bool krylov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

bool krylov::Point::operator<(const Point& other) const
{
  return std::tie(x, y) < std::tie(other.x, other.y);
}

bool krylov::Polygon::operator==(const Polygon& other) const
{
  return points == other.points;
}

bool krylov::VertexesCmp::operator()(const Polygon& polygon) const
{
  return polygon.points.size() == k;
}

bool krylov::IntersectCmp::operator()(const Polygon& other) const
{
  auto left = std::minmax_element(polygon.points.begin(), polygon.points.end());
  auto right = std::minmax_element(other.points.begin(), other.points.end());
  return !(*left.second < *right.first || *right.second < *left.first);
}

bool krylov::isEven(const Polygon &polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool krylov::isOdd(const Polygon &polygon)
{
  return !isEven(polygon);
}

double krylov::getArea(const Polygon& polygon)
{
  const std::vector< Point >& p = polygon.points;
  if (p.empty())
  {
    return 0.0;
  }
  double area = crossProduct(p.back(), p.front());
  area += std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, crossProduct);
  return std::abs(area) / 2.0;
}

double krylov::areaMean(const std::vector< Polygon >& polygons)
{
  std::vector< double > areas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
  double res = std::accumulate(areas.begin(), areas.end(), 0.0);
  return res / polygons.size();
}

bool krylov::maxArea(const Polygon& p1, const Polygon& p2)
{
  return getArea(p1) < getArea(p2);
}

bool krylov::maxVertexes(const Polygon& p1, const Polygon& p2)
{
  return p1.points.size() < p2.points.size();
}

size_t krylov::countEven(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t krylov::countOdd(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t krylov::countVertexes(const std::vector< Polygon >& polygons, size_t vert)
{
  using namespace std::placeholders;

  VertexesCmp cmp{ vert };
  return std::count_if(polygons.begin(), polygons.end(), cmp);
}

bool krylov::isPointsEqual(const Point& p1, const Point& p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

bool krylov::isPolygonsEqual(const Polygon& p1, const Polygon& p2)
{
  const std::vector< Point >& points1 = p1.points;
  const std::vector< Point >& points2 = p2.points;
  if (points1.size() != points2.size())
  {
    return false;
  }
  return std::equal(points1.begin(), points1.end(), points2.begin(), isPointsEqual);
}

int krylov::crossProduct(const krylov::Point &p1, const krylov::Point &p2)
{
  return p1.x * p2.y - p1.y * p2.x;
}

