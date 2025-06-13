#include "polygonUtils.hpp"
#include <cmath>
#include <numeric>
#include "polygon.hpp"

pilugina::PolygonAreaAccumulator::PolygonAreaAccumulator(const Polygon &p):
  poly(p),
  n(p.points.size())
{}

double pilugina::PolygonAreaAccumulator::operator()(double acc, const Point &p1) const
{
  size_t i = &p1 - &poly.points[0];
  const Point &p2 = poly.points[(i + 1) % n];
  return acc + (p1.x * p2.y - p2.x * p1.y);
}

double pilugina::getPolygonArea(const Polygon &poly)
{
  if (poly.points.size() < 3)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  double area = std::accumulate(poly.points.cbegin(), poly.points.cend(), 0.0, PolygonAreaAccumulator(poly)) / 2.0;
  return std::abs(area);
}

double pilugina::areaSumOperator(double init, const Polygon &poly)
{
  return init + getPolygonArea(poly);
}

bool pilugina::isEvenVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 0;
}

bool pilugina::isOddVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 1;
}

bool pilugina::isEqualVertexNum(std::size_t n, const Polygon &poly)
{
  return n == poly.points.size();
}
