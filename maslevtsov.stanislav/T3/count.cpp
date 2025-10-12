#include "count.hpp"
#include <functional>
#include <algorithm>
#include <map>
#include "polygon_utils.hpp"

namespace {
  std::size_t count_even_vertexes(const std::vector< maslevtsov::Polygon >& polygons)
  {
    return std::count_if(polygons.cbegin(), polygons.cend(), maslevtsov::is_even_vertex_num);
  }

  std::size_t count_odd_vertexes(const std::vector< maslevtsov::Polygon >& polygons)
  {
    return std::count_if(polygons.cbegin(), polygons.cend(), maslevtsov::is_odd_vertex_num);
  }
}

void maslevtsov::count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< std::size_t() > > subcommands;
  using namespace std::placeholders;
  subcommands["EVEN"] = std::bind(count_even_vertexes, std::cref(polygons));
  subcommands["ODD"] = std::bind(count_odd_vertexes, std::cref(polygons));

  std::string subcommand;
  in >> subcommand;
  try {
    out << subcommands.at(subcommand)();
  } catch (const std::out_of_range&) {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3) {
      throw std::invalid_argument("invalid polygon");
    }
    auto same_vertex_num = std::bind(is_equal_vertex_num, vertex_num, std::placeholders::_1);
    out << std::count_if(polygons.cbegin(), polygons.cend(), same_vertex_num);
  }
}
