#include "commands.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <iterator>
#include <vector>
#include <numeric>
#include <limits>
#include "polygon.h"
#include <guard.h>

namespace
{
  bool compareDouble(double d1, double d2, double epsilon)
  {
    return std::abs(d1 - d2) < epsilon;
  }

  struct pointDeltaSum
  {
    double operator()(const kharlamov::Point& a, const kharlamov::Point& b) const
    {
      return (a.x - b.x) + (a.y - b.y);
    }
  };

  struct CalcAreaTerm
  {
    double operator()(const kharlamov::Point& p1, const kharlamov::Point& p2)
    {
      return p1.x * p2.y - p2.x * p1.y;
    }
  };

  double getArea(const kharlamov::Polygon& polygon)
  {
    const auto points = polygon.points;
    const kharlamov::Point first = points.front();
    const kharlamov::Point last = points.back();
    double area = std::inner_product(
    points.begin(), points.end() - 1, points.begin() + 1, CalcAreaTerm()(last, first), std::plus< double >(), CalcAreaTerm());
    return std::abs(area) / 2.0;
  }

  bool isEven(const kharlamov::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOdd(const kharlamov::Polygon& poly)
  {
    return !isEven(poly);
  }

  bool acceptAll(const kharlamov::Polygon&)
  {
    return true;
  }
  struct VertexPred
  {
    size_t count;
    bool operator()(const kharlamov::Polygon& poly)
    {
      return poly.points.size() == count;
    }
  };

  template< typename Predicate >
  double areaSum(const std::vector< kharlamov::Polygon >& polygons, Predicate p)
  {
    std::vector< kharlamov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  double areaEven(const std::vector< kharlamov::Polygon >& polygons)
  {
    return areaSum(polygons, isEven);
  }
  double areaOdd(const std::vector< kharlamov::Polygon >& polygons)
  {
    return areaSum(polygons, isOdd);
  }
  double areaMean(const std::vector< kharlamov::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    return areaSum(polygons, acceptAll) / polygons.size();
  }
  double areaNum(const std::vector< kharlamov::Polygon >& polygons, size_t n)
  {
    return areaSum(polygons, VertexPred{ n });
  }

  bool compareVertex(const kharlamov::Polygon& poly1, const kharlamov::Polygon& poly2)
  {
    return poly1.points.size() < poly2.points.size();
  }

  bool compareArea(const kharlamov::Polygon& poly1, const kharlamov::Polygon& poly2)
  {
    return getArea(poly1) < getArea(poly2);
  }

  void maxArea(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareArea));
    out << std::fixed << std::setprecision(1) << getArea(max) << "\n";
  }

  void maxVertex(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareVertex));
    out << max.points.size() << "\n";
  }

  void minArea(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareArea));
    out << std::fixed << std::setprecision(1) << getArea(min) << "\n";
  }

  void minVertex(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareVertex));
    out << min.points.size() << "\n";
  }

  template< typename Predicate >
  size_t countIf(const std::vector< kharlamov::Polygon >& polygons, Predicate p)
  {
    std::vector< kharlamov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    return filtered.size();
  }

  void countEven(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isEven) << "\n";
  }

  void countOdd(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isOdd) << "\n";
  }

  void countNum(const std::vector< kharlamov::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << countIf(polygons, VertexPred{ n }) << "\n";
  }
  struct isSame
  {
    kharlamov::Polygon& p;
    bool operator()(const kharlamov::Polygon& src) const
    {
      if (src.points.size() != p.points.size())
      {
        return false;
      }
      using namespace std::placeholders;
      double deltaX = p.points[0].x - src.points[0].x;
      double deltaY = p.points[0].y - src.points[0].y;
      double absoluteDelta = deltaX + deltaY;
      std::vector< double > sumOfDeltaCoords;
      std::transform(p.points.begin(), p.points.end(), src.points.begin(), std::back_inserter(sumOfDeltaCoords), pointDeltaSum{});
      size_t counterOfPoints = std::count_if(sumOfDeltaCoords.begin(), sumOfDeltaCoords.end(), std::bind(compareDouble, _1, absoluteDelta, 1e-9));
      return counterOfPoints == sumOfDeltaCoords.size();
    }
  };
}

void kharlamov::doAreaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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
  Guard scope(out);
  out << std::fixed << std::setprecision(1) << res << "\n";
}

void kharlamov::doMaxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void kharlamov::doMinCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void kharlamov::doCountCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
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

void kharlamov::doMaxSeqCommand(std::istream& in, std::ostream& out, const PolygonVec& polygons)
{
  Polygon target;
  in >> target;
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t maxCount = 0;
  size_t currentCount = 0;
  for (const auto& poly : polygons)
  {
    if (poly == target)
    {
      currentCount++;
      maxCount = std::max(maxCount, currentCount);
    }
    else
    {
      currentCount = 0;
    }
  }
  out << maxCount << "\n";
}

void kharlamov::doSameCommand(std::istream& in, std::ostream& out, const PolygonVec& polygons)
{
  Polygon polygon;
  in >> polygon;
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("error polygon");
  }
  out << std::count_if(polygons.begin(), polygons.end(), isSame{ polygon }) << '\n';
}
