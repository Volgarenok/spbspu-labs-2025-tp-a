#include "functors.hpp"
#include "polygon.hpp"

namespace shiryaeva
{
  bool IsEven::operator()(const Polygon& polygon) const
  {
    return polygon.points.size() % 2 == 0;
  }

  bool IsOdd::operator()(const Polygon& polygon) const
  {
    return polygon.points.size() % 2 != 0;
  }

  bool HasVertexCount::operator()(const Polygon& polygon) const
  {
    return polygon.points.size() == count;
  }

  double GetPolygonArea::operator()(const Polygon& polygon) const
  {
    return getArea(polygon);
  }

  size_t GetPolygonVertexCount::operator()(const Polygon& polygon) const
  {
    return polygon.points.size();
  }
}
