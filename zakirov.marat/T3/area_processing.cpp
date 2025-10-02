#include "area_processing.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <limits>
#include <stream_guardian.hpp>
#include "utilities.hpp"

void zakirov::process_area(const std::list< Polygon > & plgns, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  Guardian guard(out);
  if (subcommand == "EVEN")
  {
    out << std::fixed << std::setprecision(1) << count_sum_area(plgns, even_polygon_pred);
  }
  else if (subcommand == "ODD")
  {
    out << std::fixed << std::setprecision(1) << count_sum_area(plgns, odd_polygon_pred);
  }
  else if (subcommand == "MEAN")
  {
    std::vector< double > areas;
    std::transform(plgns.begin(), plgns.end(), std::back_inserter(areas), count_area);
    size_t size = areas.size();
    Guardian guard(out);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    if (result != 0.0)
    {
      out << std::fixed << std::setprecision(1) << std::accumulate(areas.begin(), areas.end(), 0.0) / size;
    }
    else
    {
      throw std::logic_error("Division by zero");
    }
  }
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t a = std::stoull(subcommand);
    if (a < 3)
    {
      throw std::logic_error("Division by zero");
    }

    std::vector< Polygon > polygons;
    std::copy_if(plgns.begin(), plgns.end(), std::back_inserter(polygons), std::bind(equal_vertexes_pred, std::placeholders::_1, a));
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), count_area);
    out << std::fixed << std::setprecision(1) << std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}

void zakirov::process_less_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  if (points.empty())
  {
    throw std::logic_error("There are no figures to processing area");
  }

  Guardian guard(out);
  std::string subcommand;
  in >> subcommand;
  Polygon plgn;
  if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t count_vertexes = std::stoull(subcommand);
    if (count_vertexes < 3)
    {
      throw std::invalid_argument("Invalid figure");
    }

    Point pnt;
    while (in.peek() != '\n' && in >> pnt)
    {
      plgn.points_.push_back(pnt);
    }

    if (in.fail() || plgn.points_.size() != count_vertexes)
    {
      throw std::invalid_argument("Invalid figure");
    }

    double base_area = count_area(plgn);
    std::vector< double > areas;
    std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
    std::vector< double > less_areas;
    out << std::count_if(areas.begin(), areas.end(), std::bind(less_area_pred, base_area, std::placeholders::_1));
  }
}
