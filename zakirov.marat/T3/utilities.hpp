#ifndef UTILITES_HPP
#define UTILITES_HPP
#include "geometric_shape.hpp"
#include <list>
#include <numeric>
#include <algorithm>

namespace zakirov
{
  bool odd_polygon_pred(const Polygon & polygon);
  bool even_polygon_pred(const Polygon & polygon);
  bool less_area_pred(double area_base, double area_other);
  bool equal_vertexes_pred(const zakirov::Polygon & polygon, size_t vertex_base);
  size_t get_vertex(const Polygon & polygon);
  double count_area(const Polygon & plgn);
  int extract_num_until(const std::string & str, size_t start_pos, char el);
  Point parce_point (const std::string & str);
  template <class P>
  double count_sum_area(const std::list< Polygon > & points, P pred);

  template <class P>
  double zakirov::count_sum_area(const std::list< zakirov::Polygon > & points, P pred)
  {
    std::vector< Polygon > polygons;
    std::copy_if(points.begin(), points.end(),  std::back_inserter(polygons), pred);
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), count_area);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}

#endif
