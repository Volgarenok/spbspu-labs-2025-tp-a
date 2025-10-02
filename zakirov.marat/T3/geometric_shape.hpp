#ifndef GEOMETRIC_SHAPE_HPP
#define GEOMETRIC_SHAPE_HPP
#include <iostream>
#include <vector>
#include <utility>

namespace zakirov
{
  class Point
  {
  public:
    int x_;
    int y_;
  };

  class Polygon
  {
  public:
    Polygon() = default;
    std::vector< Point > points_;
  };

  std::istream & operator>>(std::istream & in, Point & point);
  std::istream & operator>>(std::istream & in, Polygon & polygon);
  std::ostream & operator<<(std::ostream & out, Point & point);
  bool operator==(const Point & point_base, const Point & point_other);
}

#endif
