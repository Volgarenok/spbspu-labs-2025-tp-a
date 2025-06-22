#include "polygonfunctors.hpp"
#include <cmath>
#include <functional>
#include <algorithm>

bool shapkov::isSize(const Polygon& p, size_t size)
{
  return p.points.size() == size;
}

bool shapkov::compareDouble(double d1, double d2, double epsilon)
{
  return std::abs(d1 - d2) < epsilon;
}

double shapkov::pointDeltaSum::operator()(const Point& a, const Point& b) const
{
  return (a.x - b.x) + (a.y - b.y);
}

bool shapkov::isSame::operator()(const Polygon& src) const
{
  if (src.points.size() != p.points.size())
  {
    return false;
  }
  using namespace std::placeholders;
  double deltaX = p.points[0].x - src.points[0].x;
  double deltaY = p.points[0].y - src.points[0].y;
  double absoluteDelta = deltaX + deltaY;
  std::vector< double > sumOfDeltaCoords;
  std::transform(p.points.begin(), p.points.end(), src.points.begin(), std::back_inserter(sumOfDeltaCoords), pointDeltaSum{});
  size_t counterOfPoints = std::count_if(sumOfDeltaCoords.begin(), sumOfDeltaCoords.end(), std::bind(compareDouble, _1, absoluteDelta, 1e-9));
  return counterOfPoints == sumOfDeltaCoords.size();
}
