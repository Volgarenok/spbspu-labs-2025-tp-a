#include "minCommands.hpp"
#include <format_guard.hpp>
#include <functional>
#include <map>
#include <numeric>
#include <iomanip>
#include "polygonUtils.hpp"

double pilugina::updateMinArea(double currentMin, const Polygon &poly)
{
  return std::min(currentMin, getPolygonArea(poly));
}

double pilugina::updateMinVertices(std::size_t currentMin, const Polygon &poly)
{
  return std::min(currentMin, poly.points.size());
}

void pilugina::printMinArea(const std::vector< Polygon > &polys, std::ostream &out)
{
  FormatGuard g(out);
  out << std::setprecision(1) << std::accumulate(polys.cbegin(), polys.cend(), 0.0, updateMinArea);
}

void pilugina::printMinVertices(const std::vector< Polygon > &polys, std::ostream &out)
{
  out << std::accumulate(polys.cbegin(), polys.cend(), 0ull, updateMinVertices);
}

void pilugina::printMin(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
{
  if (polys.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  using std::placeholders::_1;
  std::map< std::string, std::function< void(std::ostream &) > > subcommands{
      {"AREA", std::bind(printMinArea, std::cref(polys), _1)},
      {"VERTEXES", std::bind(printMinVertices, std::cref(polys), _1)}};

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}
