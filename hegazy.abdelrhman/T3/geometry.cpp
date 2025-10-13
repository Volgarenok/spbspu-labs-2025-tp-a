#include "geometry.hpp"
#include "DataStruct.h"

#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>

std::istream &bob::operator>>(std::istream &in, bob::Point &p)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  bob::Point temp{0, 0};
  in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};

  if (in)
    p = temp;
  return in;
}

std::istream &bob::operator>>(std::istream &in, bob::Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  size_t count;
  in >> count;
  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< bob::Point > temp;
  using input_it_t = std::istream_iterator< bob::Point >;
  std::copy_n(input_it_t{in}, count, std::back_inserter(temp));

  if (in)
  {
    polygon.points = temp;
  }
  return in;
}

bool bob::isOdd(const bob::Polygon &p)
{
  return (p.points.size() % 2) == 1;
}

bool bob::isEven(const bob::Polygon &p)
{
  return (p.points.size() % 2) == 0;
}

double bob::getDistance(const bob::Point &a, const bob::Point &b)
{
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

bob::Polygon bob::buildTriangle(size_t i, const std::vector< bob::Point > &pts)
{
  return bob::Polygon{std::vector< bob::Point >{pts[0], pts[i + 1], pts[i + 2]}};
}

bob::Polygon bob::TriangleGenerator::operator()()
{
  return buildTriangle(index++, points);
}

std::vector< bob::Polygon > bob::polygonToTriangles(const bob::Polygon &poly)
{
  std::vector< bob::Polygon > triangles(poly.points.size() - 2);
  size_t index = 0;
  std::generate(triangles.begin(), triangles.end(), TriangleGenerator{index, poly.points});
  return triangles;
}

double bob::getTriangleArea(const bob::Polygon &triangle)
{
  double a = getDistance(triangle.points[0], triangle.points[1]);
  double b = getDistance(triangle.points[1], triangle.points[2]);
  double c = getDistance(triangle.points[0], triangle.points[2]);
  double s = (a + b + c) / 2.0;
  return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

double bob::getPolygonArea(const bob::Polygon &poly)
{
  std::vector< bob::Polygon > triangles = polygonToTriangles(poly);
  std::vector< double > areas(triangles.size());
  std::transform(triangles.begin(), triangles.end(), areas.begin(), getTriangleArea);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

double bob::getTotalArea(const std::vector< bob::Polygon > &polygons)
{
  std::vector< double > areas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), areas.begin(), getPolygonArea);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool bob::compareByArea(const Polygon &a, const Polygon &b)
{
  return getPolygonArea(a) < getPolygonArea(b);
}

bool bob::compareByVertexes(const Polygon &a, const Polygon &b)
{
  return a.points.size() < b.points.size();
}

bool bob::comparePointByX(const Point &a, const Point &b)
{
  return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool bob::comparePointByY(const Point &a, const Point &b)
{
  return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}
