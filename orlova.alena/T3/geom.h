#ifndef GEOM_H
#define GEOM_H

#include <vector>

namespace orlova
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
