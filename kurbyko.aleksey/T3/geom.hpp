#ifndef GEOM_HPP
#define GEOM_HPP

#include <vector>
#include <iostream>

namespace kurbyko
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

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  std::ostream& operator<<(std::ostream&, const Point&);
  std::ostream& operator<<(std::ostream&, const Polygon&);
  bool operator==(const Point&, const Point&);
  bool operator==(const Polygon&, const Polygon&);
}

#endif
