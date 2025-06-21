#include "areaCommands.hpp"
#include <functional>
#include <map>
#include <numeric>
#include <algorithm>
#include <vector>
#include "polygon.hpp"
#include "polygonUtils.hpp"

double pilugina::calcAreasSum(const std::vector< Polygon > &polys)
{
  return std::accumulate(polys.cbegin(), polys.cend(), 0.0, areaSumOperator);
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

double pilugina::getMeanArea(const std::vector< Polygon > &polys)
{
  if (polys.empty())
  {
    throw std::invalid_argument("no polys");
  }
  return calcAreasSum(polys) / polys.size();
}

double pilugina::calcAreas(const std::vector< Polygon > &polys, std::istream &in)
{
  using std::placeholders::_1;
  std::map< std::string, std::function< double() > > subcommands
  {
      {"EVEN", std::bind(getEvenArea, std::cref(polys))},
      {"ODD", std::bind(getOddArea, std::cref(polys))},
      {"MEAN", std::bind(getMeanArea, std::cref(polys))}
  };

  std::string subcommand;
  in >> subcommand;

  double result;
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (const std::out_of_range &)
  {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< Polygon > filtered;
    auto sameVerticesNum = std::bind(isEqualVertexNum, vertex_num, _1);
    std::copy_if(polys.cbegin(), polys.cend(), std::back_inserter(filtered), sameVerticesNum);
    result = calcAreasSum(filtered);
  }
  return result;
}
