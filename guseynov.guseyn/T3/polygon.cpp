#include "polygon.hpp"
#include <algorithm>

bool guseynov::Polygon::operator==(const Polygon& other) const
{
  return points.size() == other.points.size() && std::equal(points.begin(), points.end(), other.points.begin());
}

std::istream& guseynov::operator>>(std::istream& in, Polygon& poly)
{
  poly.points.clear();
  size_t numVertices = 0;
  if (!(in >> numVertices))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (numVertices < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  for (size_t i = 0; i < numVertices; ++i)
  {
    Point point;
    if (!(in >> point))
    {
      in.setstate(std::ios::failbit);
      poly.points.clear();
      return in;
    }
    poly.points.push_back(point);
  }
  return in;
}
