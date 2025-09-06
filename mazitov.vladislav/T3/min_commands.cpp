#include "min_commands.hpp"
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

  bool areaLess(const Polygon &a, const Polygon &b)
  {
    return getPolygonArea(a) < getPolygonArea(b);
  }

  bool vertexCountLess(const Polygon &a, const Polygon &b)
  {
    return a.points.size() < b.points.size();
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

  void printMinArea(const std::vector< Polygon > &polys, std::ostream &out)
  {
    auto minIt = std::min_element(polys.begin(), polys.end(), areaLess);
    printPolygonArea(*minIt, out);
  }

  void printMinVertices(const std::vector< Polygon > &polys, std::ostream &out)
  {
    auto minIt = std::min_element(polys.begin(), polys.end(), vertexCountLess);
    printVertexCount(*minIt, out);
  }
}

void mazitov::printMin(const std::vector< Polygon > &polys, std::istream &in, std::ostream &out)
{
  if (polys.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  using std::placeholders::_1;
  std::map< std::string, std::function< void(std::ostream &) > > subcommands
  {
    {"AREA", std::bind(printMinArea, std::cref(polys), _1)},
    {"VERTEXES", std::bind(printMinVertices, std::cref(polys), _1)}
  };

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}
