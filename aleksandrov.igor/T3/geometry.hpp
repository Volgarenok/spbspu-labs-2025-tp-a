#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

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
}

#endif

