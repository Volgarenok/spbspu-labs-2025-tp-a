#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <iostream>
#include <vector>
#include <common_io_actions.hpp>

namespace tkach
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream& operator>>(std::istream& in, Point& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);
  std::ostream& operator<<(std::ostream& out, const Point& dest);
  std::ostream& operator<<(std::ostream& out, const Polygon& dest);
}

#endif
