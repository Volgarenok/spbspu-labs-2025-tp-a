#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H
#include "shapes.h"

namespace savintsev
{
  double calc_polygon_area(const Polygon a);

  int is_lines_int(const Point m1, const Point m2, const Point n1, const Point n2);
  bool is_bbox_int(const Point m1, const Point m2, const Point n1, const Point n2);
  bool is_on_segment(const Point p, const Point p1, const Point p2);
}

#endif
