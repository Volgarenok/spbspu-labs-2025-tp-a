#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <istream>
#include <ostream>
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

  std::istream& operator>>(std::istream&, Polygon&);
}

#endif
