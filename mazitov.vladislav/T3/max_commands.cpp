#include "max_commands.hpp"
#include <format_guard.hpp>
#include <functional>
#include <map>
#include <numeric>
#include <iomanip>
#include <stdexcept>
#include <format_guard.hpp>

namespace
{
  using mazitov::Polygon;
  using mazitov::FormatGuard;

  struct MaxAreaAccumulator
  {
    double operator()(double currentMax, const Polygon &poly) const
    {
      return std::max(currentMax, getPolygonArea(poly));
    }
  };

  struct MaxVerticesAccumulator
  {
    std::size_t operator()(std::size_t currentMax, const Polygon &poly) const
    {
      return std::max(currentMax, poly.points.size());
    }
  };

  void printMaxArea(const std::vector< Polygon > &polys, std::ostream &out)
  {
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << std::accumulate(polys.begin(), polys.end(), 0.0, MaxAreaAccumulator());
  }

  void printMaxVertices(const std::vector< Polygon > &polys, std::ostream &out)
  {
    out << std::accumulate(polys.begin(), polys.end(), 0ull, MaxVerticesAccumulator());
  }
}

void mazitov::printMax(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
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
