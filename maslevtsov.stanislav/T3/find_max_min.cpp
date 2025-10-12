#include "find_max_min.hpp"
#include <functional>
#include <algorithm>
#include <map>
#include <io_fmt_guard.hpp>
#include "polygon_utils.hpp"

namespace {
  void print_max_area(const std::vector< maslevtsov::Polygon >& polygons, std::ostream& out)
  {
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), maslevtsov::get_polygon_area);
    maslevtsov::IOFmtGuard guard(out);
    out << std::fixed << std::setprecision(1) << *std::max_element(areas.cbegin(), areas.cend());
  }

  void print_max_vertexes(const std::vector< maslevtsov::Polygon >& polygons, std::ostream& out)
  {
    out << std::max_element(polygons.cbegin(), polygons.cend(), maslevtsov::compare_vertex_num_less)->points.size();
  }

  void print_min_area(const std::vector< maslevtsov::Polygon >& polygons, std::ostream& out)
  {
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), maslevtsov::get_polygon_area);
    maslevtsov::IOFmtGuard guard(out);
    out << std::fixed << std::setprecision(1) << *std::min_element(areas.cbegin(), areas.cend());
  }

  void print_min_vertexes(const std::vector< maslevtsov::Polygon >& polygons, std::ostream& out)
  {
    out << std::min_element(polygons.cbegin(), polygons.cend(), maslevtsov::compare_vertex_num_less)->points.size();
  }
}

void maslevtsov::find_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::invalid_argument("no polygons");
  }

  std::map< std::string, std::function< void(std::ostream&) > > subcommands;
  using namespace std::placeholders;
  subcommands["AREA"] = std::bind(print_max_area, std::cref(polygons), _1);
  subcommands["VERTEXES"] = std::bind(print_max_vertexes, std::cref(polygons), _1);

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}

void maslevtsov::find_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::invalid_argument("no polygons");
  }
  std::map< std::string, std::function< void(std::ostream&) > > subcommands;
  using namespace std::placeholders;
  subcommands["AREA"] = std::bind(print_min_area, std::cref(polygons), _1);
  subcommands["VERTEXES"] = std::bind(print_min_vertexes, std::cref(polygons), _1);

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}
