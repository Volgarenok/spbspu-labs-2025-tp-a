#include "commands.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <vector>
#include <numeric>
#include <limits>
#include "polygon.hpp"
#include "scopeGuard.hpp"

namespace
{
  bool isEven(const kiselev::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOdd(const kiselev::Polygon& poly)
  {
    return !isEven(poly);
  }

  bool acceptAll(const kiselev::Polygon&)
  {
    return true;
  }
  struct VertexPred
  {
    size_t count;
    bool operator()(const kiselev::Polygon& poly)
    {
      return poly.points.size() == count;
    }
  };

  template< typename Predicate >
  double areaSum(const std::vector< kiselev::Polygon >& polygons, Predicate p)
  {
    std::vector< kiselev::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), kiselev::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  double areaEven(const std::vector< kiselev::Polygon >& polygons)
  {
    return areaSum(polygons, isEven);
  }
  double areaOdd(const std::vector< kiselev::Polygon >& polygons)
  {
    return areaSum(polygons, isOdd);
  }
  double areaMean(const std::vector< kiselev::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    return areaSum(polygons, acceptAll) / polygons.size();
  }
  double areaNum(const std::vector< kiselev::Polygon >& polygons, size_t n)
  {
    return areaSum(polygons, VertexPred{ n });
  }

  bool compareVertex(const kiselev::Polygon& poly1, const kiselev::Polygon& poly2)
  {
    return poly1.points.size() < poly2.points.size();
  }

  bool compareArea(const kiselev::Polygon& poly1, const kiselev::Polygon& poly2)
  {
    return kiselev::getArea(poly1) < kiselev::getArea(poly2);
  }

  void maxArea(const std::vector< kiselev::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareArea));
    detail::ScopeGuard scope(out);
    out << std::fixed << std::setprecision(1) << kiselev::getArea(max) << "\n";
  }

  void maxVertex(const std::vector< kiselev::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareVertex));
    out << max.points.size() << "\n";
  }

  void minArea(const std::vector< kiselev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareArea));
    detail::ScopeGuard scope(out);
    out << std::fixed << std::setprecision(1) << kiselev::getArea(min) << "\n";
  }

  void minVertex(const std::vector< kiselev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareVertex));
    out << min.points.size() << "\n";
  }

  template< typename Predicate >
  size_t countIf(const std::vector< kiselev::Polygon >& polygons, Predicate p)
  {
    std::vector< kiselev::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    return filtered.size();
  }

  void countEven(const std::vector< kiselev::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isEven) << "\n";
  }

  void countOdd(const std::vector< kiselev::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isOdd) << "\n";
  }

  void countNum(const std::vector< kiselev::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << countIf(polygons, VertexPred{ n }) << "\n";
  }
}

void kiselev::doAreaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  double res = 0.0;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(areaOdd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(areaMean, std::cref(polygons));
  try
  {
    res = subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    res = areaNum(polygons, n);
  }
  detail::ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << res << "\n";
}

void kiselev::doMaxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(maxArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(maxVertex, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void kiselev::doMinCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(minArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(minVertex, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void kiselev::doCountCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, std::cref(polygons), std::ref(out));
  subcommands["ODD"] = std::bind(countOdd, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    countNum(polygons, out, n);
  }
}

void kiselev::doRectsCommand(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), isRect) << "\n";
}

void kiselev::doLessAreaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon poly;
  in >> poly;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  using namespace std::placeholders;
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(compareArea, _1, poly)) << "\n";
}
