#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <ios>
#include <vector>

namespace aleksandrov
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
}

#endif

