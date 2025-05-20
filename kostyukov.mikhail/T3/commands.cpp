#include "commands.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "geometry.hpp"
#include "scopeGuard.hpp"

namespace
{
  bool isEven(const kostyukov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const kostyukov::Polygon& polygon)
  {
    return !isEven(polygon);
  }

  bool isPermutation(const kostyukov::Polygon& polygon1, const kostyukov::Polygon& polygon2)
  {
    if (polygon1.points.size() != polygon2.points.size())
    {
      return false;
    }
    return std::is_permutation(polygon1.points.cbegin(), polygon1.points.cend(), polygon2.points.cbegin());
  }

  bool forAll(const kostyukov::Polygon&)
  {
    return true;
  }

  struct VertexExpected
  {
    size_t count;
    bool operator()(const kostyukov::Polygon& polygon) const
    {
      return polygon.points.size() == count;
    }
  };

  template< typename Parameter >
  double sumAreasIf(const std::vector< kostyukov::Polygon >& polygons, Parameter param)
  {
    std::vector< kostyukov::Polygon > suitablePolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(suitablePolygons), param);
    std::vector< double > areas;
    std::transform(suitablePolygons.begin(), suitablePolygons.end(), std::back_inserter(areas), kostyukov::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double areaForEven(const std::vector< kostyukov::Polygon >& polygons)
  {
    return sumAreasIf(polygons, isEven);
  }

  double areaForOdd(const std::vector< kostyukov::Polygon >& polygons)
  {
    return sumAreasIf(polygons, isOdd);
  }

  double areaForNum(const std::vector< kostyukov::Polygon >& polygons, size_t countVertexes)
  {
    return sumAreasIf(polygons, VertexExpected{ countVertexes });
  }

  double meanArea(const std::vector< kostyukov::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("no polygons for mean area");
    }
    return sumAreasIf(polygons, forAll) / polygons.size();
  }

  template < typename Parameter >
  size_t countPolygonsIf(const std::vector< kostyukov::Polygon >& polygons, Parameter param)
  {
    std::vector< kostyukov::Polygon > suitablePolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(suitablePolygons), param);
    return suitablePolygons.size();
  }

  size_t countForEven(const std::vector< kostyukov::Polygon >& polygons)
  {
    return countPolygonsIf(polygons, isEven);
  }

  size_t countForOdd(const std::vector< kostyukov::Polygon >& polygons)
  {
    return countPolygonsIf(polygons, isOdd);
  }

  size_t countForNum(const std::vector< kostyukov::Polygon >& polygons, size_t countVertexes)
  {
    return countPolygonsIf(polygons, VertexExpected{ countVertexes });
  }

  bool areaComparator(const kostyukov::Polygon& polygon1, const kostyukov::Polygon& polygon2)
  {
    return kostyukov::getArea(polygon1) < kostyukov::getArea(polygon2);
  }

  bool vertexesComparator(const kostyukov::Polygon& polygon1, const kostyukov::Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  double maxForArea(const std::vector< kostyukov::Polygon >& polygons)
  {
    kostyukov::Polygon max = (*std::max_element(polygons.begin(), polygons.end(), areaComparator));
    return kostyukov::getArea(max);
  }

  size_t maxForVertexes(const std::vector< kostyukov::Polygon >& polygons)
  {
    kostyukov::Polygon max = (*std::max_element(polygons.begin(), polygons.end(), vertexesComparator));
    return max.points.size();
  }

  double minForArea(const std::vector< kostyukov::Polygon >& polygons)
  {
    kostyukov::Polygon min = (*std::min_element(polygons.begin(), polygons.end(), areaComparator));
    return kostyukov::getArea(min);
  }

  size_t minForVertexes(const std::vector< kostyukov::Polygon >& polygons)
  {
    kostyukov::Polygon min = (*std::min_element(polygons.begin(), polygons.end(), vertexesComparator));
    return min.points.size();
  }

  struct rightAnglesInspector
  {
    const kostyukov::Polygon polygon;
    bool operator()(size_t number) const
    {
      size_t countVertexes = polygon.points.size();
      const kostyukov::Point& previous = polygon.points[(number + countVertexes - 1) % countVertexes];
      const kostyukov::Point& current = polygon.points[number];
      const kostyukov::Point& next = polygon.points[(number + 1) % countVertexes];
      double vector1_x = current.x - previous.x;
      double vector1_y = current.y - previous.y;
      double vector2_x = next.x - current.x;
      double vector2_y = next.y - current.y;
      double scalarProduct = vector1_x * vector2_x + vector1_y * vector2_y;
      return scalarProduct == 0;
    }
  };

  struct AccumulatorRightAngles
  {
    const kostyukov::Polygon polygon;
    bool operator()(bool alreadyHasRightAngle, size_t number) const
    {
      if (alreadyHasRightAngle)
      {
        return true;
      }
      return rightAnglesInspector{ polygon }(number);
    }
  };

  bool hasRightAngles(const kostyukov::Polygon& polygon)
  {
    std::vector< size_t > numbers(polygon.points.size());
    std::iota(numbers.begin(), numbers.end(), 0);
    return std::accumulate(numbers.begin(), numbers.end(), false, AccumulatorRightAngles{ polygon });
  }
}

void kostyukov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  double result = 0.0;
  if (subcommand == "EVEN")
  {
    result = areaForEven(polygons);
  }
  else if (subcommand == "ODD")
  {
    result = areaForOdd(polygons);
  }
  else if (subcommand == "MEAN")
  {
    result = meanArea(polygons);
  }
  else
  {
    size_t countVertexes = 0;
    try
    {
      countVertexes = std::stoull(subcommand);
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("invalid subcommand or number format");
    }
    if (countVertexes < 3)
    {
      throw std::invalid_argument("polygon index must be 3 or greater");
    }
    result = areaForNum(polygons, countVertexes);
  }
  ScopeGuard scopeGrd(out);
  out << std::fixed << std::setprecision(1) << result;
  return;
}

void kostyukov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("no polygons for max");
  }
  std::string subcommand;
  in >> subcommand;
  kostyukov::ScopeGuard scopeGrd(out);
  if (subcommand == "AREA")
  {
    out << std::fixed << std::setprecision(1) << maxForArea(polygons);
  }
  else if (subcommand == "VERTEXES")
  {
    out << std::fixed << std::setprecision(1) << maxForVertexes(polygons);
  }
  else
  {
    throw std::invalid_argument("invalid subcommand");
  }
  return;
}

void kostyukov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("no polygons for min");
  }
  std::string subcommand;
  in >> subcommand;
  kostyukov::ScopeGuard scopeGrd(out);
  if (subcommand == "AREA")
  {
    out << std::fixed << std::setprecision(1) << minForArea(polygons);
  }
  else if (subcommand == "VERTEXES")
  {
    out << std::fixed << std::setprecision(1) << minForVertexes(polygons);
  }
  else
  {
    throw std::invalid_argument("invalid subcommand");
  }
  return;
}

void kostyukov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  size_t result = 0;
  if (subcommand == "EVEN")
  {
    result = countForEven(polygons);
  }
  else if (subcommand == "ODD")
  {
    result = countForOdd(polygons);
  }
  else
  {
    size_t countVertexes = 0;
    try
    {
      countVertexes = std::stoull(subcommand);
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("invalid subcommand or number format");
    }
    if (countVertexes < 3)
    {
      throw std::invalid_argument("polygon must have 3 or more vertexes");
    }
    result = countForNum(polygons, countVertexes);
  }
  ScopeGuard scopeGrd(out);
  out << std::fixed << std::setprecision(1) << result;
  return;
}

void kostyukov::permsCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::invalid_argument("polygon must have 3 or more vertexes");
  }
  {
    using namespace std::placeholders;
    out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(isPermutation, _1, polygon));
  }
  return;
}

void kostyukov::rightShapesCount(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.cbegin(), polygons.cend(), hasRightAngles);
  return;
}
