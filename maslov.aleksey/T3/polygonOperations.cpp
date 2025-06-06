#include "polygonOperations.hpp"
#include <algorithm>
#include <numeric>

namespace
{
  int scalarProduct(const maslov::Point & a,
      const maslov::Point & b, const maslov::Point & c)
  {
    int vectorX1 = b.x - a.x;
    int vectorY1 = b.y - a.y;
    int vectorX2 = c.x - b.x;
    int vectorY2 = c.y - b.y;
    return vectorX1 * vectorX2 + vectorY1 * vectorY2;
  }

  struct VectorProduct
  {
    const maslov::Point p0;
    double operator()(const maslov::Point & p1, const maslov::Point & p2) const
    {
      double d1 = (p1.x - p0.x) * (p2.y - p0.y);
      double d2 = (p2.x - p0.x) * (p1.y - p0.y);
      return 0.5 * std::abs(d1 - d2);
    }
  };
}

bool maslov::IsEven::operator()(const Polygon & polygon) const
{
  return polygon.points.size() % 2 == 0;
}

bool maslov::IsOdd::operator()(const Polygon & polygon) const
{
  return polygon.points.size() % 2 != 0;
}

double maslov::AreaCalculator::operator()(const Polygon & polygon) const
{
  std::vector< double > areas;
  const auto & begin = polygon.points.begin();
  const auto & end = polygon.points.end();
  VectorProduct calc{polygon.points[0]};
  std::transform(begin + 1, end - 1, begin + 2, std::back_inserter(areas), calc);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool maslov::SameVertexes::operator()(const Polygon & polygon) const
{
  return polygon.points.size() == num;
}

bool maslov::VertexesComparator::operator()(const Polygon & poly1, const Polygon & poly2) const
{
  return poly1.points.size() < poly2.points.size();
}

bool maslov::IsRectangle::operator()(const Polygon & polygon) const
{
  if (polygon.points.size() != 4)
  {
    return false;
  }
  const auto & p = polygon.points;
  bool check = (scalarProduct(p[0], p[1], p[2]) == 0) && (scalarProduct(p[1], p[2], p[3]) == 0);
  return check && (scalarProduct(p[2], p[3], p[0]) == 0);
}

bool maslov::SamePolygon::operator()(const Polygon & polygon) const
{
  return polygon == inPolygon;
}

int maslov::EchoInserter::operator()(const Polygon & poly)
{
  result.push_back(poly);
  if (poly == inPolygon)
  {
    result.push_back(poly);
  }
  return 0;
}
