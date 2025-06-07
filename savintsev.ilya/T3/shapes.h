#ifndef SHAPES_H
#define SHAPES_H
#include <vector>
#include <iostream>

namespace savintsev
{
  struct Point
  {
    int x, y;
  };

  bool operator<(const Point & a, const Point & b);

  std::istream & operator>>(std::istream & in, Point & dest);
  std::ostream & operator<<(std::ostream & out, const Point & dest);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream & operator>>(std::istream & in, Polygon & dest);
  std::ostream & operator<<(std::ostream & out, const Polygon & dest);
}

#endif
