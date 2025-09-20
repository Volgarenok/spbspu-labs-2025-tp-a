#include "extremum_processing.hpp"
#include <algorithm>
#include <iomanip>
#include <stream_guardian.hpp>
#include "utilities.hpp"

void zakirov::find_max_extremum(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  if (points.empty())
  {
    throw std::logic_error("There are no figures to find max");
  }

  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    Guardian guard(out);
    std::vector< double > areas;
    std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
    out << std::fixed << std::setprecision(1) << *std::max_element(areas.begin(), areas.end());
  }
  else if (subcommand == "VERTEXES")
  {
    std::vector< size_t > vertexes;
    std::transform(points.begin(), points.end(), std::back_inserter(vertexes), get_vertex);
    out << *std::max_element(vertexes.begin(), vertexes.end());
  }
}

void zakirov::find_min_extremum(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  if (points.empty())
  {
    throw std::logic_error("There are no figures to find min");
  }

  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    Guardian guard(out);
    std::vector< double > areas;
    std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
    out << std::fixed << std::setprecision(1) << *std::min_element(areas.begin(), areas.end());
  }
  else if (subcommand == "VERTEXES")
  {
    std::vector< size_t > vertexes;
    std::transform(points.begin(), points.end(), std::back_inserter(vertexes), get_vertex);
    out << *std::min_element(vertexes.begin(), vertexes.end());
  }
}
