#include "polygon.hpp"
#include <algorithm>

bool guseynov::Polygon::operator==(const Polygon& other) const
{
  if (points.size() != other.points.size()) return false;
  return std::equal(points.begin(), points.end(), other.points.begin());
}
