#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <cmath>
#include <streamGuard.hpp>

namespace
{
  struct Triangle
  {
    shramko::Point a;
    shramko::Point b;
    shramko::Point c;
  };

  Triangle makeTriangle(const shramko::Point& first, const std::vector< shramko::Point >& points, size_t index)
  {
    return {first, points[index], points[index + 1]};
  }

  double calculateTriangleArea(const Triangle& tri)
  {
    return 0.5 * std::abs(tri.a.x * (tri.b.y - tri.c.y) + tri.b.x * (tri.c.y - tri.a.y) + tri.c.x * (tri.a.y - tri.b.y));
  }
}

std::istream& shramko::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);

  char ch;
  in >> ch;
  if (ch != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> point.x;
  in >> ch;
  if (ch != ';')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> point.y;
  in >> ch;
  if (ch != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::istream& shramko::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t numPoints = 0;
  in >> numPoints;
  if (numPoints < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector<Point> temp(numPoints);
  for (size_t i = 0; i < numPoints; ++i)
  {
    if (!(in >> temp[i]))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (in && in.peek() == '\n')
  {
    polygon.points = std::move(temp);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

double shramko::getPolygonArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }
  const Point& first = polygon.points[0];
  size_t numTriangles = polygon.points.size() - 2;
  std::vector< double > areas(numTriangles);
  for (size_t i = 0; i < numTriangles; ++i)
  {
    Triangle tri = makeTriangle(first, polygon.points, i + 1);
    areas[i] = calculateTriangleArea(tri);
  }
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}
