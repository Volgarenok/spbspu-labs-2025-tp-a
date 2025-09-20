#include "subcommands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include "ScopeGuard.hpp"
#include "polygon.hpp"

namespace
{
  bool checkPolygonVertexesAmount(const holodilov::Polygon& polygon, const size_t amountVertexes)
  {
    return polygon.points.size() == amountVertexes;
  }

  bool checkPolygonVertexesEven(const holodilov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool checkPolygonVertexesOdd(const holodilov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 1;
  }

  bool comparePolygonsByVertexes(const holodilov::Polygon& lhs, const holodilov::Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  bool comparePolygonsByArea(const holodilov::Polygon& lhs, const holodilov::Polygon& rhs)
  {
    return lhs.getArea() < rhs.getArea();
  }

  double accumulateAreaEvenOdd(double sum, const holodilov::Polygon& polygon, bool isOdd)
  {
    if (polygon.points.size() % 2 == isOdd)
    {
      sum += polygon.getArea();
    }
    return sum;
  }

  double accumulateArea(double sum, const holodilov::Polygon& polygon)
  {
    return sum + polygon.getArea();
  }

  double accumulateAreaVertexes(double sum, const holodilov::Polygon& polygon, size_t amountVertexes)
  {
    if (polygon.points.size() == amountVertexes)
    {
      sum += polygon.getArea();
    }
    return sum;
  }
}

namespace sc = holodilov::subcommands;

void sc::countEven(std::ostream& os, const VecPolygons& vecPolygons)
{
  size_t count = std::count_if(vecPolygons.begin(), vecPolygons.end(), checkPolygonVertexesEven);
  os << count;;
}

void sc::countOdd(std::ostream& os, const VecPolygons& vecPolygons)
{
  size_t count = std::count_if(vecPolygons.begin(), vecPolygons.end(), checkPolygonVertexesOdd);
  os << count;
}

void sc::countAmount(std::ostream& os, const VecPolygons& vecPolygons, size_t amountVertexes)
{
  auto checker = std::bind(checkPolygonVertexesAmount, std::placeholders::_1, amountVertexes);
  size_t count = std::count_if(vecPolygons.begin(), vecPolygons.end(), checker);
  os << count;
}

void sc::maxArea(std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::max_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByArea));
  os << std::fixed << std::setprecision(1);
  os << polygon.getArea();
}

void sc::maxVertexes(std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::max_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByVertexes));
  os << std::fixed << std::setprecision(1);
  os << polygon.points.size();
}

void sc::minArea(std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::min_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByArea));
  os << std::fixed << std::setprecision(1);
  os << polygon.getArea();
}

void sc::minVertexes(std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::min_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByVertexes));
  os << std::fixed << std::setprecision(1);
  os << polygon.points.size();
}

void sc::areaEven(std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  auto accumulator = std::bind(accumulateAreaEvenOdd, std::placeholders::_1, std::placeholders::_2, false);
  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, accumulator);
  os << std::fixed << std::setprecision(1);
  os << sumArea;
}

void sc::areaOdd(std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  auto accumulator = std::bind(accumulateAreaEvenOdd, std::placeholders::_1, std::placeholders::_2, true);
  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, accumulator);
  os << std::fixed << std::setprecision(1);
  os << sumArea;
}

void sc::areaMean(std::ostream& os, const VecPolygons& vecPolygons)
{
  if (vecPolygons.empty())
  {
    throw std::logic_error("Error: no polygons to handle.\n");
  }

  ScopeGuard scopeGuard(os);

  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, accumulateArea);
  double meanArea = sumArea / static_cast< double >(vecPolygons.size());
  os << std::fixed << std::setprecision(1);
  os << meanArea;
}

void sc::areaAmount(std::ostream& os, const VecPolygons& vecPolygons, size_t amountVertexes)
{
  ScopeGuard scopeGuard(os);

  auto accumulator = std::bind(accumulateAreaVertexes, std::placeholders::_1, std::placeholders::_2, amountVertexes);
  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, accumulator);
  os << std::fixed << std::setprecision(1);
  os << sumArea;
}

