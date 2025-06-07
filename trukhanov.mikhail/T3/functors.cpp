#include "functors.hpp"
#include <algorithm>
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
