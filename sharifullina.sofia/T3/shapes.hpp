#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <vector>
#include <istream>

namespace sharifullina
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point & rhs) const;
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon & rhs) const;
  };

  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);
}

#endif
