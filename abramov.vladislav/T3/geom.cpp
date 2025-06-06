#include "geom.hpp"
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>

namespace
{
  struct DelimiterIO
  {
    char delim;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.delim))
    {
      throw std::logic_error("wrong symbol\n");
    }
    return in;
  }

  int pointsSum(const abramov::Point &p1, const abramov::Point &p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  bool isEven(const abramov::Polygon &polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const abramov::Polygon &polygon)
  {
    return !isEven(polygon);
  }

  bool isEnoughVertexes(const abramov::Polygon &polygon, size_t k)
  {
    return polygon.points.size() == k;
  }
}

std::istream &abramov::operator>>(std::istream &in, Point &p)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point tmp{ 0, 0 };
  in >> DelimiterIO{ '(' };
  in >> tmp.x;
  in >> DelimiterIO{ ';' };
  in >> tmp.y;
  in >> DelimiterIO{ ')' };
  if (in)
  {
    p = tmp;
  }
  else
  {
    throw std::logic_error("Fail to read\n");
  }
  return in;
}

std::istream &abramov::operator>>(std::istream &in, Polygon &polygon)
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
    throw std::logic_error("Wrong size\n");
  }
  std::vector< Point > pts(k);
  std::copy_n(std::istream_iterator< Point >(in), k, pts.begin());
  if (!in)
  {
    throw std::logic_error("Fail to read\n");
  }
  polygon.points = pts;
  return in;
}

double abramov::getArea(const Polygon &polygon)
{
  const std::vector< Point > &p = polygon.points;
  double area = pointsSum(p.back(), p.front());
  area += std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, pointsSum);
  return std::abs(area) / 2.0;
}

double abramov::areaEven(double s, const Polygon &polygon)
{
  if (isEven(polygon))
  {
    return s + getArea(polygon);
  }
  return s;
}

double abramov::areaOdd(double s, const Polygon &polygon)
{
  if (!isEven(polygon))
  {
    return s + getArea(polygon);
  }
  return s;
}

double abramov::areaMean(const std::vector< Polygon > &polygons)
{
  double s1 = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaEven);
  double s2 = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaOdd);
  return (s1 + s2) / polygons.size();
}

double abramov::areaVertexes(double s, const Polygon &polygon, size_t vert)
{
  if (polygon.points.size() == vert)
  {
    return s + getArea(polygon);
  }
  return s;
}

bool abramov::maxArea(const Polygon &p1, const Polygon &p2)
{
  return getArea(p1) < getArea(p2);
}

bool abramov::maxVertexes(const Polygon &p1, const Polygon &p2)
{
  return p1.points.size() < p2.points.size();
}

size_t abramov::countEven(const std::vector< Polygon > &polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t abramov::countOdd(const std::vector< Polygon > &polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t abramov::countVertexes(const std::vector< Polygon > &polygons, size_t vert)
{
  using namespace std::placeholders;

  auto f = std::bind(isEnoughVertexes, _1, vert);
  return std::count_if(polygons.begin(), polygons.end(), f);
}

bool abramov::isPointsEqual(const Point &p1, const Point &p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

bool abramov::isPolygonsEqual(const Polygon &p1, const Polygon &p2)
{
  const std::vector< Point > &points1 = p1.points;
  const std::vector< Point > &points2 = p2.points;
  if (points1.size() != points2.size())
  {
    return false;
  }
  return std::equal(points1.begin(), points1.end(), points2.begin(), isPointsEqual);
}

bool abramov::isPolygonsEqualToExample(const Polygon &p1, const Polygon &p2, const Polygon &ex)
{
  return isPolygonsEqual(p1, ex) && isPolygonsEqual(p2, ex);
}
