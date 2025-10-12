#include "echo_rmecho.hpp"
#include <functional>
#include <algorithm>
#include "polygon_utils.hpp"

void maslevtsov::echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  if (!(in >> polygon)) {
    throw std::invalid_argument("invalid polygon");
  }
  auto is_same_with_arg = std::bind(is_same, polygon, std::placeholders::_1);
  std::size_t additional_size = std::count_if(polygons.begin(), polygons.end(), is_same_with_arg);
  std::vector< Polygon > with_echoes(polygons.size() + additional_size);
  std::vector< int > dump(polygons.size() + additional_size);
  auto inserter = std::bind(WithSamesInserter{with_echoes}, std::placeholders::_1, polygon);
  std::transform(polygons.begin(), polygons.end(), dump.begin(), inserter);
  polygons = with_echoes;
  out << additional_size;
}

void maslevtsov::remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  if (!(in >> polygon)) {
    throw std::invalid_argument("invalid polygon");
  }
  auto is_same_with_arg = std::bind(is_same, polygon, std::placeholders::_1);
  auto is_same_first_with_arg = std::bind(is_same_with_arg, std::placeholders::_1);
  auto is_same_second_with_arg = std::bind(is_same_with_arg, std::placeholders::_2);
  auto is_same_both_to_arg = std::bind(std::logical_and< bool >{}, is_same_first_with_arg, is_same_second_with_arg);
  auto first_to_erase = std::unique(polygons.begin(), polygons.end(), is_same_both_to_arg);
  out << std::distance(first_to_erase, polygons.end());
  polygons.erase(first_to_erase, polygons.end());
}
