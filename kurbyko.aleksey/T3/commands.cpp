#include "commands.hpp"
#include <functional>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <numeric>
#include <string>
#include "geom.hpp"
#include "iofmtguard.hpp"

namespace
{
  using namespace kurbyko;

  double subArea(const Point& a, const Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double areaPolygon(const Polygon& polygon)
  {
    const auto& p = polygon.points;
    double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
    sum += subArea(p.back(), p.front());
    return std::abs(sum) / 2.0;
  }

  bool isEven(const Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon& polygon)
  {
    return !isEven(polygon);
  }

  bool isNum(const Polygon& polygon, size_t numOfVertexes)
  {
    return polygon.points.size() == numOfVertexes;
  }

  bool noCondition(const Polygon&)
  {
    return true;
  }

  template < typename T >
  double accumulator(double sum, const Polygon& polygon, T function)
  {
    return function(polygon) ? sum + areaPolygon(polygon) : sum;
  }

  template < typename T >
  double calculateAreaByCondition(const std::vector< Polygon >& polygons, T function)
  {
    std::vector< Polygon > newPolygons;
    std::vector< double > areas;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newPolygons), function);
    std::transform(newPolygons.begin(), newPolygons.end(), std::back_inserter(areas), areaPolygon);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool areaComparator(const Polygon& a, const Polygon& b)
  {
    return areaPolygon(a) < areaPolygon(b);
  }

  bool vertexesComparator(const Polygon& a, const Polygon& b)
  {
    return a.points.size() < b.points.size();
  }

  bool isPermutation(const Polygon& polygon1, const Polygon& polygon2)
  {
    if (polygon1.points.size() != polygon2.points.size())
    {
      return false;
    }
    return std::is_permutation(polygon1.points.begin(), polygon1.points.end(), polygon2.points.begin());
  }

  struct MaxSeqCalculator
  {
    const Polygon& target;
    std::pair<size_t, size_t> operator()(std::pair<size_t, size_t> acc, const Polygon& poly) const
    {
      size_t current = acc.first;
      size_t max = acc.second;
      if (poly == target)
      {
        current++;
        max = std::max(max, current);
      }
      else
      {
        current = 0;
      }
      return std::make_pair(current, max);
    }
  };
}

double kurbyko::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  return calculateAreaByCondition(polygons, noCondition) / polygons.size();
}

double kurbyko::areaNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  auto function = std::bind(isNum, _1, numOfVertexes);
  return calculateAreaByCondition(polygons, function);
}

void kurbyko::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1);
  using namespace std::placeholders;
  using Predicate = std::function< bool(const Polygon&) >;

  std::map< std::string, std::function< double(const std::vector<Polygon>&) > > subcmds;
  subcmds["EVEN"] = std::bind(calculateAreaByCondition< Predicate >, _1, isEven);
  subcmds["ODD"] = std::bind(calculateAreaByCondition< Predicate >, _1, isOdd);
  subcmds["MEAN"] = areaMean;
  try
  {
    out << subcmds.at(subcommand)(polygons);
  }
  catch (...)
  {
    int numOfVertexes = std::stoull(subcommand);
    if (numOfVertexes < 3)
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }
    out << areaNum(polygons, numOfVertexes);
  }
}

void kurbyko::max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
  subcmds["AREA"] = maxArea;
  subcmds["VERTEXES"] = maxVertexes;
  subcmds.at(subcommand)(polygons, out);
}

void kurbyko::maxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

void kurbyko::maxVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << (*std::max_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void kurbyko::min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
  subcmds["AREA"] = minArea;
  subcmds["VERTEXES"] = minVertexes;
  subcmds.at(subcommand)(polygons, out);
}

void kurbyko::minArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

void kurbyko::minVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << (*std::min_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void kurbyko::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = countEven;
  subcmds["ODD"] = countOdd;
  try
  {
    out << subcmds.at(subcommand)(polygons);
  }
  catch (...)
  {
    size_t numOfVertexes = std::stoull(subcommand);
    if (numOfVertexes < 3)
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }
    out << countNum(polygons, numOfVertexes);
  }
}

size_t kurbyko::countEven(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t kurbyko::countOdd(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t kurbyko::countNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  return std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, _1, numOfVertexes));
}

void kurbyko::perms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("WRONG POLYGON SIZE");
  }
  using namespace std::placeholders;
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(isPermutation, _1, polygon));
}

void kurbyko::maxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon target;
  in >> target;

  if (target.points.size() < 3)
  {
    throw std::logic_error("<WRONG POLYGON SIZE>");
  }

  MaxSeqCalculator calc{ target };

  auto result = std::accumulate(polygons.begin(), polygons.end(),
    std::make_pair(static_cast<size_t>(0), static_cast<size_t>(0)), calc);

  size_t maxSeq = result.second;

  if (maxSeq == 0)
  {
    throw std::logic_error("THERE ARE NO IDENTIC POLYGONS");
  }

  out << maxSeq;
}
