#include "calc_areas.hpp"
#include <numeric>
#include <algorithm>
#include <functional>
#include <map>
#include <io_fmt_guard.hpp>
#include "polygon_utils.hpp"

namespace {
  double calc_areas_sum(const std::vector< maslevtsov::Polygon >& polygons)
  {
    using namespace maslevtsov;

    std::vector< double > to_accumulate;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(to_accumulate), get_polygon_area);
    double result = std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
    return result;
  }

  double get_even_area(const std::vector< maslevtsov::Polygon >& polygons)
  {
    using namespace maslevtsov;

    std::vector< Polygon > filtered;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), is_even_vertex_num);
    return calc_areas_sum(filtered);
  }

  double get_odd_area(const std::vector< maslevtsov::Polygon >& polygons)
  {
    using namespace maslevtsov;

    std::vector< Polygon > filtered;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), is_odd_vertex_num);
    return calc_areas_sum(filtered);
  }

  double get_mean_area(const std::vector< maslevtsov::Polygon >& polygons)
  {
    using namespace maslevtsov;

    if (polygons.empty()) {
      throw std::invalid_argument("no polygons");
    }
    return calc_areas_sum(polygons) / polygons.size();
  }
}

void maslevtsov::calc_areas(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< double() > > subcommands;
  using namespace std::placeholders;
  subcommands["EVEN"] = std::bind(get_even_area, std::cref(polygons));
  subcommands["ODD"] = std::bind(get_odd_area, std::cref(polygons));
  subcommands["MEAN"] = std::bind(get_mean_area, std::cref(polygons));

  std::string subcommand;
  in >> subcommand;
  double result;
  try {
    result = subcommands.at(subcommand)();
  } catch (const std::out_of_range&) {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3) {
      throw std::invalid_argument("invalid polygon");
    }
    std::vector< Polygon > filtered;
    auto same_vertex_num = std::bind(is_equal_vertex_num, vertex_num, std::placeholders::_1);
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), same_vertex_num);
    result = calc_areas_sum(filtered);
  }
  IOFmtGuard guard(out);
  out << std::fixed << std::setprecision(1) << result;
}
