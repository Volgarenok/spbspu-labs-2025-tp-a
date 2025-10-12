#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "Point.hpp"

namespace gavrilova {
  struct Polygon {
    std::vector< Point > points;
  };

  double getArea(const Polygon& polygon);
  bool hasEvenVertices(const Polygon& polygon);
  bool hasOddVertices(const Polygon& polygon);

  std::istream& operator>>(std::istream& is, Polygon& polygon);
  std::ostream& operator<<(std::ostream& os, const Polygon& polygon);
}

#endif
