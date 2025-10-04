#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "point.hpp"

namespace guseynov
{
  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };
}

#endif
