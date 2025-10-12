#include "polygon_utils.hpp"
#include <algorithm>
#include <numeric>
#include <functional>

bool maslevtsov::is_even_vertex_num(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool maslevtsov::is_odd_vertex_num(const Polygon& polygon)
{
  return !is_even_vertex_num(polygon);
}

bool maslevtsov::is_equal_vertex_num(std::size_t vertex_num, const Polygon& polygon)
{
  return vertex_num == polygon.points.size();
}

double maslevtsov::add_main_diagonal(const Point& lhs, const Point& rhs)
{
  return lhs.x * rhs.y;
}

double maslevtsov::subtract_sub_diagonal(const Point& lhs, const Point& rhs)
{
  return -lhs.y * rhs.x;
}

double maslevtsov::get_polygon_area(const Polygon& polygon)
{
  std::vector< Point > second_lace;
  std::copy(++polygon.points.cbegin(), polygon.points.cend(), std::back_inserter(second_lace));
  second_lace.push_back(*polygon.points.cbegin());
  std::vector< double > to_accumulate(polygon.points.size());
  auto begin_it = polygon.points.cbegin();
  auto end_it = polygon.points.cend();
  double area = 0.0;
  std::transform(begin_it, end_it, second_lace.cbegin(), to_accumulate.begin(), add_main_diagonal);
  area += std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
  std::transform(begin_it, end_it, second_lace.cbegin(), to_accumulate.begin(), subtract_sub_diagonal);
  area += std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
  area *= 0.5;
  area = std::abs(area);
  return area;
}

bool maslevtsov::compare_vertex_num_less(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool maslevtsov::is_same(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points.size() != rhs.points.size()) {
    return false;
  }
  return std::mismatch(lhs.points.cbegin(), lhs.points.cend(), rhs.points.cbegin()).first == lhs.points.cend();
}

int maslevtsov::WithSamesInserter::operator()(Polygon& polygon, const Polygon& to_compare)
{
  auto same_with_arg = std::bind(is_same, to_compare, std::placeholders::_1);
  with_echos.push_back(polygon);
  if (same_with_arg(polygon)) {
    with_echos.push_back(polygon);
  }
  return 0;
}
