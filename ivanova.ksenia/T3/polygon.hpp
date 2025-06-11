#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>

namespace ivanova
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };
}

#endif
