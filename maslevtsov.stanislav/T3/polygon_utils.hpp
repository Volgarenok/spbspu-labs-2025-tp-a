#ifndef POLYGON_UTILS_HPP
#define POLYGON_UTILS_HPP

#include "shapes.hpp"

namespace maslevtsov {
  bool is_even_vertex_num(const Polygon& polygon);
  bool is_odd_vertex_num(const Polygon& polygon);
  bool is_equal_vertex_num(std::size_t vertex_num, const Polygon& polygon);

  double add_main_diagonal(const Point& lhs, const Point& rhs);
  double subtract_sub_diagonal(const Point& lhs, const Point& rhs);
  double get_polygon_area(const Polygon& polygon);

  bool compare_vertex_num_less(const Polygon& lhs, const Polygon& rhs);
  bool is_same(const Polygon& lhs, const Polygon& rhs);

  struct WithSamesInserter
  {
    std::vector< Polygon >& with_echos;

    int operator()(Polygon& polygon, const Polygon& to_compare);
  };
}

#endif
