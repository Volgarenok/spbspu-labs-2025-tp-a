#include "min_commands.hpp"
#include <format_guard.hpp>
#include <functional>
#include <map>
#include <numeric>
#include <iomanip>
#include <stdexcept>
#include <format_guard.hpp>

namespace
{
  using mazitov::FormatGuard;
  using mazitov::Polygon;

  struct MinAreaAccumulator
  {
    double operator()(double currentMin, const Polygon &poly) const
    {
      return (currentMin == 0) ? (getPolygonArea(poly)) : (std::min(currentMin, getPolygonArea(poly)));
    }
  };

  struct MinVerticesAccumulator
  {
    std::size_t operator()(std::size_t currentMin, const Polygon &poly) const
    {
      return (currentMin == 0) ? (poly.points.size()) : (std::min(currentMin, poly.points.size()));
    }
  };

  void printMinArea(const std::vector< Polygon > &polys, std::ostream &out)
  {
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << std::accumulate(polys.begin(), polys.end(), 0.0, MinAreaAccumulator());
  }

  void printMinVertices(const std::vector< Polygon > &polys, std::ostream &out)
  {
    out << std::accumulate(polys.begin(), polys.end(), 0ull, MinVerticesAccumulator());
  }
}

void mazitov::printMin(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
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
