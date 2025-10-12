#include "commands.h"
#include <functional>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <numeric>
#include "geom.h"
#include "io-guard.h"

namespace
{
  using namespace orlova;

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

  size_t getMaxSeq(const Polygon& polygon, const Polygon& target)
  {
    size_t currentSeq = 0;
    size_t maxSeq = 0;
    if (polygon == target)
    {
      currentSeq++;
      maxSeq = std::max(maxSeq, currentSeq);
    }
    else
    {
      currentSeq = 0;
    }
    return maxSeq;
  }
}

double orlova::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  return calculateAreaByCondition(polygons, noCondition) / polygons.size();
}

double orlova::areaNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  auto function = std::bind(isNum, _1, numOfVertexes);
  return calculateAreaByCondition(polygons, function);
}

void orlova::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  IoGuard guard(out);
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

void orlova::max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  IoGuard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
  subcmds["AREA"] = maxArea;
  subcmds["VERTEXES"] = maxVertexes;
  subcmds.at(subcommand)(polygons, out);
}

void orlova::maxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

void orlova::maxVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << (*std::max_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void orlova::min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  IoGuard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
  subcmds["AREA"] = minArea;
  subcmds["VERTEXES"] = minVertexes;
  subcmds.at(subcommand)(polygons, out);
}

void orlova::minArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

void orlova::minVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << (*std::min_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void orlova::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  IoGuard guard(out);
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

size_t orlova::countEven(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t orlova::countOdd(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t orlova::countNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  return std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, _1, numOfVertexes));
}

void orlova::perms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
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

void orlova::maxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("<WRONG POLYGON SIZE>");
  }
  if (std::count(polygons.begin(), polygons.end(), polygon) == 0)
  {
    throw std::logic_error("THERE ARE NO IDENTIC POLYGONS");
  }
  std::vector< size_t > seqVector(polygons.size());
  std::transform(polygons.begin(), polygons.end(), seqVector.begin(), std::bind(getMaxSeq, _1, std::cref(polygon)));
  out << *std::max_element(seqVector.begin(), seqVector.end());
}
