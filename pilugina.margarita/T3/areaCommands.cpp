#include "areaCommands.hpp"
#include <functional>
#include <map>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include "polygon.hpp"

double pilugina::getTriangleArea(const Point &p1, const Point &p2, const Point &p3)
{
  return 0.5 * std::abs(p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
}

double pilugina::getPolygonArea(const Polygon &poly)
{
  if (poly.points.size() < 3)
  {
    return 0.0;
  }
  const Point &p0 = poly.points[0];
  using namespace std::placeholders;
  auto boundArea = std::bind(getTriangleArea, p0, _1, _2);
  auto firstP = poly.points.begin();
  auto endP = poly.points.end();
  return std::inner_product(firstP + 1, endP - 1, firstP + 2, 0.0, std::plus<>(), boundArea);
}

bool pilugina::isEvenVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 0;
}

bool pilugina::isOddVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 1;
}

bool pilugina::isEqualVertexNum(std::size_t n, const Polygon &poly)
{
  return n == poly.points.size();
}

double pilugina::calcAreasSum(const std::vector< Polygon > &polys)
{
  std::vector< double > areas(polys.size());
  std::transform(polys.cbegin(), polys.cend(), areas.begin(), getPolygonArea);
  return std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

double pilugina::getEvenArea(const std::vector< Polygon > &polys)
{
  std::vector< Polygon > filtered;
  std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), isEvenVertexNum);
  return calcAreasSum(filtered);
}

double pilugina::getOddArea(const std::vector< Polygon > &polys)
{
  std::vector< Polygon > filtered;
  std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), isOddVertexNum);
  return calcAreasSum(filtered);
}

double pilugina::printMeanArea(const std::vector< Polygon > &polys)
{
  if (polys.empty())
  {
    throw std::invalid_argument("no polys");
  }
  return calcAreasSum(polys) / polys.size();
}

double pilugina::calcAreasEqVert(const std::vector< Polygon > &polys, int vertex_num)
{
  std::vector< Polygon > filtered;
  auto sameVerticesNum = std::bind(isEqualVertexNum, vertex_num, std::placeholders::_1);
  std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), sameVerticesNum);
  return calcAreasSum(filtered);
}

double pilugina::calcAreas(const std::vector< Polygon > &polys, std::istream &in)
{
  std::map< std::string, std::function< double() > > subcommands
  {
    {"EVEN", std::bind(getEvenArea, std::cref(polys))},
    {"ODD", std::bind(getOddArea, std::cref(polys))},
    {"MEAN", std::bind(printMeanArea, std::cref(polys))},
  };

  std::string subcommand;
  in >> subcommand;
  auto it = subcommands.find(subcommand);
  if (it != subcommands.end())
  {
    return it->second();
  }
  else
  {
    int vertexCount = std::stoi(subcommand);
    if (vertexCount < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    return calcAreasEqVert(polys, vertexCount);
  }
}
