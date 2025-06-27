#include <map>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <numeric>
#include <limits>
#include <fstream>
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
      throw std::logic_error("No polygons");
    }
    return areaSum(polygons, acceptAll) / polygons.size();
  }

  double areaNum(const std::vector< khokhryakova::Polygon >& polygons, size_t angle)
  {
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
    std::vector< khokhryakova::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    return filtered.size();
  }

  void countEven(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isEven) << "\n";
  }

  void countOdd(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isOdd) << "\n";
  }

  void countNum(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << countIf(polygons, VertexPred{ n }) << "\n";
  }

  void areaMax(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out)
  {
    auto max = *std::max_element(polygons.begin(), polygons.end(), areaCompare);
    out << std::fixed << std::setprecision(1) << khokhryakova::getArea(max) << "\n";
  }

  void vertexMax(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out)
  {
    auto max = *std::max_element(polygons.begin(), polygons.end(), vertexCompare);
    out << max.points.size() << "\n";
  }

  void areaMin(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out)
  {
    auto min = *std::min_element(polygons.begin(), polygons.end(), areaCompare);
    out << std::fixed << std::setprecision(1) << khokhryakova::getArea(min) << "\n";
  }

  void vertexMin(const std::vector< khokhryakova::Polygon >& polygons, std::ostream& out)
  {
    auto min = *std::min_element(polygons.begin(), polygons.end(), vertexCompare);
    out << min.points.size() << "\n";
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
  try
  {
    if (subcommands.count(command))
    {
      out << std::fixed << std::setprecision(1) << subcommands.at(command)() << "\n";
    }
    else
    {
      size_t angles = std::stoull(command);
      if (angles < 3) throw std::logic_error("Error: vertices < 3");
      out << std::fixed << std::setprecision(1) << areaNum(polygons, angles) << "\n";
    }
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void khokhryakova::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string command;
  in >> command;
  if (polygons.empty())
  {
    throw std::logic_error("Error: not found polygons");
  }
  std::map< std::string, std::function<void() > > subcommands;
  subcommands["AREA"] = std::bind(areaMax, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(vertexMax, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(command)();
  }
  catch (...)
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
    throw std::logic_error("Error: not found polygons");
  }
  std::map< std::string, std::function<void() > > subcommands;
  subcommands["AREA"] = std::bind(areaMin, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(vertexMin, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(command)();
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void khokhryakova::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string command;
  in >> command;
  std::map< std::string, std::function<void() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, std::cref(polygons), std::ref(out));
  subcommands["ODD"] = std::bind(countOdd, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(command)();
  }
  catch (...)
  {
    size_t n = std::stoull(command);
    if (n < 3)
    {
        throw std::logic_error("Error: vertices < 3");
    }
    countNum(polygons, out, n);
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
  for (const auto& p : polygons)
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
  for (const auto& p : newPolygons)
  {
    file << p.points.size();
    for (const auto& point : p.points)
    {
      file << " (" << point.x << ";" << point.y << ")";
    }
    file << "\n";
  }
  polygons = newPolygons;
  out << count << "\n";
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
  for (const auto& p : polygons)
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
  out << maxCount << "\n";
}
