#include "functors.hpp"
#include <algorithm>
#include <cmath>
#include "polygon.hpp"

bool trukhanov::isSize::operator()(const Polygon& p) const
{
  return p.points.size() == size;
}

bool trukhanov::CompareByArea::operator()(const Polygon& lhs, const Polygon& rhs) const
{
  double lhsArea = getArea(lhs);
  double rhsArea = getArea(rhs);
  return lhsArea < rhsArea;
}

bool trukhanov::LessArea::operator()(const Polygon& p) const
{
  return getArea(p) < area;
}

double trukhanov::AccumulateAll::operator()(double sum, const Polygon& p) const
{
  return sum + getArea(p);
}

bool trukhanov::CompareByVertexes::operator()(const Polygon& lhs, const Polygon& rhs) const
{
  return compareByVertexes(lhs, rhs);
}

bool trukhanov::HasDuplicates::operator()(const Polygon& p) const
{
  if (p.points.size() < 2)
  {
    return false;
  }
  std::vector< Point > sortedPoints = p.points;
  std::sort(sortedPoints.begin(), sortedPoints.end());
  return std::adjacent_find(sortedPoints.begin(), sortedPoints.end()) != sortedPoints.end();
}

bool trukhanov::PolygonHasMinSize::operator()(const trukhanov::Polygon& p) const
{
  return p.points.size() >= 3;
}

double trukhanov::TriangleArea::operator()(const Point& a, const Point& b, const Point& c) const
{
  return std::abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
}

trukhanov::PolygonAreaSum::PolygonAreaSum(const Polygon& polygon) : polygon_(polygon) {}

double trukhanov::PolygonAreaSum::operator()(double sum, size_t i) const
{
  const Point& a = polygon_.points[0];
  const Point& b = polygon_.points[i];
  const Point& c = polygon_.points[i + 1];

  TriangleArea areaCalc;
  return sum + areaCalc(a, b, c);
}
