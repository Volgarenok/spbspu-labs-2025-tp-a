#include "maxCommands.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include <iomanip>
#include "areaCommands.hpp"

void pilugina::printMaxArea(const std::vector< Polygon > &polys, std::ostream &out)
{
  std::vector< double > areas(polys.size());
  std::transform(polys.cbegin(), polys.cend(), areas.begin(), getPolygonArea);
  out << std::fixed << std::setprecision(1) << *std::max_element(areas.cbegin(), areas.cend());
}

void pilugina::printMaxVertices(const std::vector< Polygon > &polys, std::ostream &out)
{
  auto it = std::max_element(polys.begin(), polys.end());
  out << it->points.size();
}

void pilugina::printMax(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
{
  if (polys.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::map< std::string, std::function< void() > > subcommands
  {
    {"AREA", std::bind(printMaxArea, std::cref(polys), std::ref(out))},
    {"VERTEXES", std::bind(printMaxVertices, std::cref(polys), std::ref(out))}
  };

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)();
}
