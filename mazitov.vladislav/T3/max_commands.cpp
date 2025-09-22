#include "max_commands.hpp"
#include <format_guard.hpp>
#include <algorithm>
#include <functional>
#include <map>
#include <iomanip>
#include <stdexcept>

namespace
{
  using mazitov::FormatGuard;
  using mazitov::getPolygonArea;
  using mazitov::Polygon;

  bool areaGreater(const Polygon &a, const Polygon &b)
  {
    return getPolygonArea(a) < getPolygonArea(b); // for max_element
  }

  bool vertexCountGreater(const Polygon &a, const Polygon &b)
  {
    return a.points.size() < b.points.size(); // for max_element
  }

  void printPolygonArea(const Polygon &poly, std::ostream &out)
  {
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << getPolygonArea(poly);
  }

  void printVertexCount(const Polygon &poly, std::ostream &out)
  {
    out << poly.points.size();
  }

  void printMaxArea(const std::vector< Polygon > &polys, std::ostream &out)
  {
    auto maxIt = std::max_element(polys.begin(), polys.end(), areaGreater);
    printPolygonArea(*maxIt, out);
  }

  void printMaxVertices(const std::vector< Polygon > &polys, std::ostream &out)
  {
    auto maxIt = std::max_element(polys.begin(), polys.end(), vertexCountGreater);
    printVertexCount(*maxIt, out);
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
