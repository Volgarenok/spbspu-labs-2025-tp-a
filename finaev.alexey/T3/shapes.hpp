#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <vector>
#include <iostream>
#include <ioData.hpp>

namespace finaev
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point&);
  std::ostream& operator<<(std::ostream& out, const Point&);

  std::istream& operator>>(std::istream& in, Polygon&);
  std::ostream& operator<<(std::ostream& out, const Polygon&);
}

#endif
