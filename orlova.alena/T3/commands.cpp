#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <functional>
#include <numeric>
#include "commands.h"
#include "io-utils.h"

double orlova::subArea(const Point& a, const Point& b)
{
  return a.x * b.y - a.y * b.x;
}

double orlova::areaPolygon(const Polygon& polygon)
{
  const auto& p = polygon.points;
  double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
  sum += subArea (p.back(), p.front());
  return std::abs(sum) / 2.0;
}

bool orlova::isEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool orlova::isOdd(const Polygon& polygon)
{
  return polygon.points.size() % 2 != 0;
}

bool orlova::isNum(const Polygon& polygon, size_t numOfVertexes)
{
  return polygon.points.size() == numOfVertexes;
}

double orlova::oddAreaAccumulator(double sum, const Polygon& polygon)
{
  return isOdd(polygon) ? sum + areaPolygon(polygon) : sum;
}

double orlova::evenAreaAccumulator(double sum, const Polygon& polygon)
{
  return isEven(polygon) ? sum + areaPolygon(polygon) : sum;
}

double orlova::meanAreaAccumulator(double sum, const Polygon& polygon)
{
  return sum + areaPolygon(polygon);
}

double orlova::numAreaAccumulator(double sum, const Polygon& polygon, size_t numOfVertexes)
{
  return isNum(polygon, numOfVertexes) ? sum + areaPolygon(polygon) : sum;
}

double orlova::areaEven(const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, evenAreaAccumulator);
}

double orlova::areaOdd(const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, oddAreaAccumulator);
}

double orlova::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, meanAreaAccumulator) / polygons.size();
}

double orlova::areaNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(numAreaAccumulator, _1, _2, numOfVertexes));
}

void orlova::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector<Polygon>&) > > subcmds;
  subcmds["EVEN"] = areaEven;
  subcmds["ODD"] = areaOdd;
  subcmds["MEAN"] = areaMean;
  auto it = subcmds.find(subcommand);
  if (it != subcmds.end())
  {
    out << it->second(polygons);
  }
  else
  {
    size_t numOfVertexes = std::stoull(subcommand);
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
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > subcmds1;
  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds2;
  subcmds1["AREA"] = maxArea;
  subcmds2["VERTEXES"] = maxVertexes;
  auto it1 = subcmds1.find(subcommand);
  if (it1 != subcmds1.end())
  {
    out << it1->second(polygons);
  }
  else
  {
    auto it2 = subcmds2.find(subcommand);
    if (it2 != subcmds2.end())
    {
      out << it2->second(polygons);
    }
    else
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }
  }
}

bool orlova::areaComparator(const Polygon& a, const Polygon& b)
{
  return areaPolygon(a) < areaPolygon(b);
}

double orlova::maxArea(const std::vector< Polygon >& polygons)
{
  return areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

bool orlova::vertexesComparator(const Polygon& a, const Polygon& b)
{
  return a.points.size() < b.points.size();
}

size_t orlova::maxVertexes(const std::vector< Polygon >& polygons)
{
  return (*std::max_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void orlova::min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > subcmds;
  subcmds["AREA"] = maxArea;
  subcmds["VERTEXES"] = maxVertexes;
  auto it = subcmds.find(subcommand);
  if (it != subcmds.end())
  {
    out << it->second(polygons);
  }
  else
  {
    throw std::logic_error("<WRONG SUBCOMMAND>");
  }
}

double orlova::minArea(const std::vector< Polygon >& polygons)
{
  return areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

size_t orlova::minVertexes(const std::vector< Polygon >& polygons)
{
  return (*std::min_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void orlova::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = countEven;
  subcmds["ODD"] = countOdd;
  auto it = subcmds.find(subcommand);
  if (it != subcmds.end())
  {
    out << it->second(polygons);
  }
  else
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
  return;
}

bool orlova::isPermutation(const Polygon& polygon1, const Polygon& polygon2)
{
  if (polygon1.points.size() != polygon2.points.size())
  {
    return false;
  }
  return std::is_permutation(polygon1.points.begin(), polygon1.points.end(), polygon2.points.begin());
}

void orlova::maxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("<WRONG POLYGON SIZE>");
  }
  int current = 0, max = 0;
  countSequence(polygons, polygon, 0, current, max);
  out << max;
  return;
}

void orlova::countSequence(const std::vector< Polygon >& polygons, const Polygon& polygon, size_t index, int& current, int& max)
{
  if (std::count(polygons.begin(), polygons.end(), polygon) == 0)
  {
    throw std::logic_error("THERE ARE NO IDENTIC POLYGONS");
  }
  if (index >= polygons.size())
  {
    return;
  }
  if (polygons[index] == polygon)
  {
    current++;
    max = std::max(max, current);
  }
  else
  {
    current = 0;
  }
  countSequence(polygons, polygon, index + 1, current, max);
}
