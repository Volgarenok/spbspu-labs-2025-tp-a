#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>
#include "point.hpp"

namespace duhanina
{
  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Polygon& lhs, const Polygon& rhs);
  bool operator!=(const Polygon& lhs, const Polygon& rhs);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  double calculateArea(const Polygon& polygon);
}

#endif
