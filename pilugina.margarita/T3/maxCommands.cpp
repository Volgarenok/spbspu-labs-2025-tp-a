#include "maxCommands.hpp"
#include <format_guard.hpp>
#include <functional>
#include <map>
#include <numeric>
#include <iomanip>
#include "polygonUtils.hpp"

double pilugina::updateMaxArea(double currentMax, const Polygon &poly)
{
  return std::max(currentMax, getPolygonArea(poly));
}

double pilugina::updateMaxVertices(std::size_t currentMax, const Polygon &poly)
{
  return std::max(currentMax, poly.points.size());
}

void pilugina::printMaxArea(const std::vector< Polygon > &polys, std::ostream &out)
{
  FormatGuard g(out);
  out << std::fixed << std::setprecision(1) << std::accumulate(polys.begin(), polys.end(), 0.0, updateMaxArea);
}

void pilugina::printMaxVertices(const std::vector< Polygon > &polys, std::ostream &out)
{
  out << std::accumulate(polys.begin(), polys.end(), 0ull, updateMaxVertices);
}

void pilugina::printMax(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
{
  if (polys.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  using std::placeholders::_1;
  std::map< std::string, std::function< void(std::ostream &) > > subcommands
  {
    {"AREA", std::bind(printMaxArea, std::cref(polys), _1)},
    {"VERTEXES", std::bind(printMaxVertices, std::cref(polys), _1)}
  };

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}
