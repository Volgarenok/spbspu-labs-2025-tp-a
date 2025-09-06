#include "area_commands.hpp"
#include <functional>
#include <map>
#include <numeric>
#include <algorithm>
#include <vector>
#include "polygon.hpp"

double mazitov::calcAreasSum(const std::vector< Polygon > &polygons)
{
  std::vector<double> polygonAreas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), polygonAreas.begin(), getPolygonArea);
  return std::accumulate(polygonAreas.cbegin(), polygonAreas.cend(), 0.0);
}

double mazitov::getEvenArea(const std::vector< Polygon > &polygons)
{
  std::vector< Polygon > filtered;
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), isEvenVertexNum);
  return calcAreasSum(filtered);
}

double mazitov::getOddArea(const std::vector< Polygon > &polygons)
{
  std::vector< Polygon > filtered;
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), isOddVertexNum);
  return calcAreasSum(filtered);
}

double mazitov::getMeanArea(const std::vector< Polygon > &polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  return calcAreasSum(polygons) / polygons.size();
}

double mazitov::calcAreas(const std::vector< Polygon > &polygons, std::istream &in)
{
  using std::placeholders::_1;
  std::map< std::string, std::function< double() > > subcommands
  {
    {"EVEN", std::bind(getEvenArea, std::cref(polygons))},
    {"ODD", std::bind(getOddArea, std::cref(polygons))},
    {"MEAN", std::bind(getMeanArea, std::cref(polygons))}
  };

  std::string subcommand;
  in >> subcommand;

  double result;
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
  {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< Polygon > filtered;
    auto vertexNumPred = std::bind(isEqualVertexNum, vertex_num, _1);
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), vertexNumPred);
    result = calcAreasSum(filtered);
  }
  return result;
}
