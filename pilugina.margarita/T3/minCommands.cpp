#include "minCommands.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include <iomanip>
#include "areaCommands.hpp"

void pilugina::printMinArea(const std::vector< Polygon > &polys, std::ostream &out)
{
  std::vector< double > areas(polys.size());
  std::transform(polys.cbegin(), polys.cend(), areas.begin(), getPolygonArea);
  out << std::fixed << std::setprecision(1) << *std::min_element(areas.cbegin(), areas.cend());
}

void pilugina::printMinVertices(const std::vector< Polygon > &polys, std::ostream &out)
{
  auto it = std::min_element(polys.begin(), polys.end());
  out << it->points.size();
}

void pilugina::printMin(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
{
  if (polys.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::map< std::string, std::function< void() > > subcommands
  {
    {"AREA", std::bind(printMinArea, std::cref(polys), std::ref(out))},
    {"VERTEXES", std::bind(printMinVertices, std::cref(polys), std::ref(out))}
  };

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)();
}
