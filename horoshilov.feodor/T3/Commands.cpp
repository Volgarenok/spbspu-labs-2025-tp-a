#include "Commands.h"
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
#include "Polygon.h"
#include <Guard.h>
namespace
{
  struct CalcGausTerm
  {
   double operator()(const horoshilov::Point& p1, const horoshilov::Point& p2)
   {
    return p1.x * p2.y - p2.x * p1.y;
   }
  };

  double getArea(const horoshilov::Polygon& polygon)
  {
  const auto points = polygon.points;
  const horoshilov::Point first = points.front();
  const horoshilov::Point last = points.back();
  double area = std::inner_product(
  points.begin(), points.end() - 1, points.begin() + 1, CalcGausTerm()(last, first), std::plus< double >(), CalcGausTerm());
  return std::abs(area) / 2.0;
  }

  bool isEven(const horoshilov::Polygon& polyg)
  {
    return polyg.points.size() % 2 == 0;
  }

  bool isOdd(const horoshilov::Polygon& polyg)
  {
    return !isEven(polyg);
  }

  bool acceptAll(const horoshilov::Polygon&)
  {
    return true;
  }

  struct VertexPred
  {
    size_t count;
    bool operator()(const horoshilov::Polygon& poly)
    {
      return poly.points.size() == count;
    }
  };

  template< typename Predicate >
  double areaSum(const std::vector< horoshilov::Polygon >& polygons, Predicate p)
  {
    std::vector< horoshilov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  double areaEven(const std::vector< horoshilov::Polygon >& polygons)
  {
    return areaSum(polygons, isEven);
  }
  double areaOdd(const std::vector< horoshilov::Polygon >& polygons)
  {
    return areaSum(polygons, isOdd);
  }
  double areaMean(const std::vector< horoshilov::Polygon >& polygons)
  {
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  return areaSum(polygons, acceptAll) / polygons.size();
  }
  double areaNum(const std::vector< horoshilov::Polygon >& polygons, size_t n)
  {
    return areaSum(polygons, VertexPred{ n });
  }

  bool compareVertex(const horoshilov::Polygon& poly1, const horoshilov::Polygon& poly2)
  {
    return poly1.points.size() < poly2.points.size();
  }

  bool compareArea(const horoshilov::Polygon& poly1, const horoshilov::Polygon& poly2)
  {
    return getArea(poly1) < getArea(poly2);
  }

  void maxArea(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareArea));
    horoshilov::Guard streamguard(out);
    out << std::fixed << std::setprecision(1) << getArea(max) << "\n";
  }

  void maxVertex(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareVertex));
    out << max.points.size() << "\n";
  }

  void minArea(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareArea));
    horoshilov::Guard streamguard(out);
    out << std::fixed << std::setprecision(1) << getArea(min) << "\n";
  }

  void minVertex(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareVertex));
    out << min.points.size() << "\n";
  }

  template< typename Predicate >
  size_t countIf(const std::vector< horoshilov::Polygon >& polygons, Predicate p)
  {
    std::vector< horoshilov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    return filtered.size();
  }

  void countEven(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isEven) << "\n";
  }

  void countOdd(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isOdd) << "\n";
  }

  void countNum(const std::vector< horoshilov::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << countIf(polygons, VertexPred{ n }) << "\n";
  }
  bool cheackPerms(const horoshilov::Polygon& polygon1, const horoshilov::Polygon& polygon2)
  {
    if (polygon1.points.size() != polygon2.points.size())
    {
      return false;
    }
    return std::is_permutation(polygon1.points.cbegin(), polygon1.points.cend(), polygon2.points.cbegin());
  }
}
void horoshilov::printIntersections(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& polygons)
{
  horoshilov::Polygon target;
  in >> target;

  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  struct IntersectsWith
  {
    const horoshilov::Polygon& polygon;
    bool operator()(const horoshilov::Polygon& other) const
    {
      auto left = std::minmax_element(polygon.points.begin(), polygon.points.end());
      auto right = std::minmax_element(other.points.begin(), other.points.end());
      return !(*left.second < *right.first || *right.second < *left.first);
    }
  };

  out << std::count_if(polygons.begin(), polygons.end(), IntersectsWith{ target }) << "\n";
}

void horoshilov::printPermsCount(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& plgs)
{
  horoshilov::Polygon plg;
  in >> plg;
  if (plg.points.size() < 3)
  {
    throw std::invalid_argument("incorrect shape");
  }
  out << std::count_if(plgs.cbegin(), plgs.cend(), std::bind(cheackPerms, std::placeholders::_1, plg)) << "\n";
}

void horoshilov::printArea(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& polygons)
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
  horoshilov::Guard scope(out);
  out << std::fixed << std::setprecision(1) << res << "\n";
}

void horoshilov::printMax(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& polygons)
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

void horoshilov::printMin(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& polygons)
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
  subcommands.at(subcommand)();
}

void horoshilov::printCount(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& polygons)
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
