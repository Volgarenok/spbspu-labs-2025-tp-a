#include <limits>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <string>
#include "commands.h"
#include "point.h"

namespace
{
  bool isEven(const khokhryakova::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOdd(const khokhryakova::Polygon& poly)
  {
    return !isEven(poly);
  }

  bool acceptAll(const khokhryakova::Polygon&)
  {
    return true;
  }

  struct VertexPred
  {
    size_t count;
    bool operator()(const khokhryakova::Polygon& poly)
    {
      return poly.points.size() == count;
    }
  };

  template< typename Predicate >
  double areaSum(const std::vector< khokhryakova::Polygon >& polygons, Predicate p)
  {
    std::vector< khokhryakova::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), khokhryakova::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double areaEven(const std::vector< khokhryakova::Polygon >& polygons)
  {
    return areaSum(polygons, isEven);
  }

  double areaOdd(const std::vector< khokhryakova::Polygon >& polygons)
  {
    return areaSum(polygons, isOdd);
  }

  double areaMedian(const std::vector< khokhryakova::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    return areaSum(polygons, acceptAll) / polygons.size();
  }

  double areaNum(const std::vector< khokhryakova::Polygon >& polygons, size_t angle)
  {
    if (angle < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    return areaSum(polygons, VertexPred{ angle });
  }

  bool vertexCompare(const khokhryakova::Polygon& polygon1, const khokhryakova::Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  bool areaCompare(const khokhryakova::Polygon& polygon1, const khokhryakova::Polygon& polygon2)
  {
    return khokhryakova::getArea(polygon1) < khokhryakova::getArea(polygon2);
  }

  template< typename Predicate >
  size_t countIf(const std::vector< khokhryakova::Polygon >& polygons, Predicate p)
  {
    return std::count_if(polygons.begin(), polygons.end(), p);
  }

  size_t countEven(const std::vector< khokhryakova::Polygon >& polygons)
  {
    return countIf(polygons, isEven);
  }

  size_t countOdd(const std::vector< khokhryakova::Polygon >& polygons)
  {
    return countIf(polygons, isOdd);
  }

  size_t countNum(const std::vector< khokhryakova::Polygon >& polygons, size_t n)
  {
    if (n < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    return countIf(polygons, VertexPred{ n });
  }

  double areaMax(const std::vector< khokhryakova::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto max = *std::max_element(polygons.begin(), polygons.end(), areaCompare);
    return khokhryakova::getArea(max);
  }

  size_t vertexMax(const std::vector< khokhryakova::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto max = *std::max_element(polygons.begin(), polygons.end(), vertexCompare);
    return max.points.size();
  }

  double areaMin(const std::vector< khokhryakova::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto min = *std::min_element(polygons.begin(), polygons.end(), areaCompare);
    return khokhryakova::getArea(min);
  }

  size_t vertexMin(const std::vector< khokhryakova::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto min = *std::min_element(polygons.begin(), polygons.end(), vertexCompare);
    return min.points.size();
  }
}

void khokhryakova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string command;
  in >> command;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(areaOdd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(areaMedian, std::cref(polygons));
  double size = 0;
  try
  {
    size = subcommands.at(command)();
  }
  catch (const std::exception&)
  {
    size_t angles = std::stoull(command);
    if (angles < 3)
    {
      throw std::logic_error("Error: vertices < 3");
    }
    size = areaNum(polygons, angles);
  }
  out << std::fixed << std::setprecision(1) << size;
}

void khokhryakova::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string command;
  in >> command;
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (command == "AREA")
  {
    out << std::fixed << std::setprecision(1) << areaMax(polygons);
  }
  else if (command == "VERTEXES")
  {
    out << vertexMax(polygons);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void khokhryakova::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string command;
  in >> command;
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (command == "AREA")
  {
    out << std::fixed << std::setprecision(1) << areaMin(polygons);
  }
  else if (command == "VERTEXES")
  {
    out << vertexMin(polygons);
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void khokhryakova::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string command;
  in >> command;
  if (command == "EVEN")
  {
    out << countEven(polygons);
  }
  else if (command == "ODD")
  {
    out << countOdd(polygons);
  }
  else
  {
    try
    {
      size_t n = std::stoull(command);
      out << countNum(polygons, n);
    }
    catch(const std::out_of_range&)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    catch (const std::invalid_argument&)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
}

void khokhryakova::echo(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons, const std::string& filename)
{
  Polygon poly;
  in >> poly;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t count = 0;
  std::vector< Polygon > newPolygons;
  for (const auto& p: polygons)
  {
    newPolygons.push_back(p);
    if (p == poly)
    {
      newPolygons.push_back(p);
      ++count;
    }
  }
  std::ofstream file(filename.c_str());
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file for writing");
  }
  for (const auto& p: newPolygons)
  {
    file << p.points.size();
    for (const auto& point: p.points)
    {
      file << " (" << point.x << ";" << point.y << ")";
    }
    file << "\n";
  }
  polygons = newPolygons;
  out << count;
}

void khokhryakova::maxSeq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon poly;
  in >> poly;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t maxCount = 0;
  size_t currentCount = 0;
  for (const auto& p: polygons)
  {
    if (p == poly)
    {
      ++currentCount;
      maxCount = std::max(maxCount, currentCount);
    }
    else
    {
      currentCount = 0;
    }
  }
  out << maxCount;
}
