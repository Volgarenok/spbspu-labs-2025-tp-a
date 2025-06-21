#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <string>
#include "geometry.hpp"
#include <stream_guard.hpp>

namespace
{
  using geom::Polygon;

  bool hasEvenVertices(const Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool hasOddVertices(const Polygon& poly)
  {
    return !hasEvenVertices(poly);
  }

  struct VertexCountMatcher
  {
    size_t targetCount;

    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() == targetCount;
    }
  };

  size_t countEvenVertices(const std::vector< Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), hasEvenVertices);
  }

  size_t countOddVertices(const std::vector< Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), hasOddVertices);
  }

  size_t countByVertexCount(const std::vector< Polygon >& polygons, size_t count)
  {
    return std::count_if(polygons.begin(), polygons.end(), VertexCountMatcher{ count });
  }

  bool compareByVertexCount(const Polygon& lhs, const Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  bool compareByArea(const Polygon& lhs, const Polygon& rhs)
  {
    return geom::getPolygonArea(lhs) < geom::getPolygonArea(rhs);
  }

  using PolygonPredicate = std::function< bool(const Polygon&) >;

  std::vector< double > extractAreas(const std::vector< Polygon >& polygons, const PolygonPredicate& pred)
  {
    std::vector< double > areas;
    std::vector< Polygon > plgs;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(plgs), pred);
    std::transform(plgs.begin(), plgs.end(), std::back_inserter(areas), geom::getPolygonArea);
    return areas;
  }

  double sumAreasByPredicate(const std::vector< Polygon >& polygons, const PolygonPredicate& pred)
  {
    auto areas = extractAreas(polygons, pred);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double totalEvenArea(const std::vector< Polygon >& polygons)
  {
    return sumAreasByPredicate(polygons, hasEvenVertices);
  }

  double totalOddArea(const std::vector< Polygon >& polygons)
  {
    return sumAreasByPredicate(polygons, hasOddVertices);
  }

  double totalAreaByVertexCount(const std::vector< Polygon >& polygons, size_t count)
  {
    return sumAreasByPredicate(polygons, VertexCountMatcher{ count });
  }

  bool acceptAll(const Polygon&)
  {
    return true;
  }

  double totalAreaAll(const std::vector< Polygon >& polygons)
  {
    return sumAreasByPredicate(polygons, acceptAll);
  }

  struct AverageAreaCalculator
  {
    double operator()(const std::vector< Polygon >& polygons) const
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("No polygons");
      }
      return totalAreaAll(polygons) / polygons.size();
    }
  };

  void outputMaxVertices(std::ostream& os, const std::vector< Polygon >& polygons)
  {
    auto  max_poly = *std::max_element(polygons.begin(), polygons.end(), compareByVertexCount);
    os << max_poly.points.size();
  }

  void outputMaxArea(std::ostream& os, const std::vector< Polygon >& polygons)
  {
    auto max_poly = *std::max_element(polygons.begin(), polygons.end(), compareByArea);
    os << std::fixed << std::setprecision(1) << geom::getPolygonArea(max_poly);
  }

  void outputMinVertices(std::ostream& os, const std::vector< Polygon >& polygons)
  {
    auto min_poly = *std::min_element(polygons.begin(), polygons.end(), compareByVertexCount);
    os << min_poly.points.size();
  }

  void outputMinArea(std::ostream& os, const std::vector< Polygon >& polygons)
  {
    auto min_poly = *std::min_element(polygons.begin(), polygons.end(), compareByArea);
    os << std::fixed << std::setprecision(1) << geom::getPolygonArea(min_poly);
  }
}

void smirnov::printAreaSum(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  std::unordered_map< std::string, std::function< double() > > handlers;
  handlers["EVEN"] = std::bind(totalEvenArea, std::cref(polygons));
  handlers["ODD"] = std::bind(totalOddArea, std::cref(polygons));
  handlers["MEAN"] = std::bind(AverageAreaCalculator{ }, std::cref(polygons));

  std::string param;
  input >> param;

  double result{};
  try
  {
    result = handlers.at(param)();
  }
  catch (...)
  {
    size_t vertices = std::stoull(param);
    if (vertices < 3)
    {
      throw std::invalid_argument("Vertex count too small");
    }
    result = totalAreaByVertexCount(polygons, vertices);
  }

  smirnov::Iofmtguard guard(output);
  output << std::fixed << std::setprecision(1) << result;
}

void smirnov::printMaxValueOf(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  std::string param;
  input >> param;

  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons available");
  }

  std::unordered_map< std::string, std::function< void() > > handlers;
  handlers["AREA"] = std::bind(outputMaxArea, std::ref(output), std::cref(polygons));
  handlers["VERTEXES"] = std::bind(outputMaxVertices, std::ref(output), std::cref(polygons));

  smirnov::Iofmtguard guard(output);

  try
  {
    handlers.at(param)();
  }
  catch (...)
  {
    throw std::invalid_argument("Unknown command");
  }
}

void smirnov::printMinValueOf(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  std::string param;
  input >> param;

  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons available");
  }

  std::unordered_map< std::string, std::function< void() > > handlers;
  handlers["AREA"] = std::bind(outputMinArea, std::ref(output), std::cref(polygons));
  handlers["VERTEXES"] = std::bind(outputMinVertices, std::ref(output), std::cref(polygons));

  smirnov::Iofmtguard guard(output);

  try
  {
    handlers.at(param)();
  }
  catch (...)
  {
    throw std::invalid_argument("Unknown command");
  }
}

void smirnov::printCountOf(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  std::unordered_map< std::string, std::function< size_t() > > handlers;
  handlers["EVEN"] = std::bind(countEvenVertices, std::cref(polygons));
  handlers["ODD"] = std::bind(countOddVertices, std::cref(polygons));

  std::string param;
  input >> param;

  size_t count{};
  try
  {
    count = handlers.at(param)();
  }
  catch (...)
  {
    size_t vertices = std::stoull(param);
    if (vertices < 3)
    {
      throw std::invalid_argument("Invalid vertex count parameter");
    }
    count = countByVertexCount(polygons, vertices);
  }

  output << count;
}

void smirnov::printLessAreaCnt(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  Polygon ref;
  input >> ref;
  if (ref.points.size() < 3)
  {
    throw std::invalid_argument("Invalid polygon");
  }

  const double refArea = geom::getPolygonArea(ref);
  output << std::count_if(polygons.begin(), polygons.end(),
    [refArea](const Polygon& plg)
    {
      return geom::getPolygonArea(plg) < refArea;
    });
}

bool smirnov::polygonsIntersect(const Polygon& p1, const Polygon& p2)
{
  auto minmaxX1 = std::minmax_element(p1.points.begin(), p1.points.end(),
    [](const geom::Point& a, const geom::Point& b) { return a.x < b.x; });
  auto minmaxY1 = std::minmax_element(p1.points.begin(), p1.points.end(),
    [](const geom::Point& a, const geom::Point& b) { return a.y < b.y; });

  auto minmaxX2 = std::minmax_element(p2.points.begin(), p2.points.end(),
    [](const geom::Point& a, const geom::Point& b) { return a.x < b.x; });
  auto minmaxY2 = std::minmax_element(p2.points.begin(), p2.points.end(),
    [](const geom::Point& a, const geom::Point& b) { return a.y < b.y; });

  return !(minmaxX1.second->x < minmaxX2.first->x || minmaxX2.second->x < minmaxX1.first->x ||
    minmaxY1.second->y < minmaxY2.first->y || minmaxY2.second->y < minmaxY1.first->y);
}


void smirnov::printIntersectionsCnt(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output)
{
  Polygon ref;
  input >> ref;
  if (ref.points.size() < 3)
  {
    throw std::invalid_argument("Invalid polygon");
  }

  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [&ref](const Polygon& plg)
    {
      return smirnov::polygonsIntersect(plg, ref);
    });

  output << count;
}

