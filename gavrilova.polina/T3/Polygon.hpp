#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "Point.hpp"

namespace gavrilova {

  struct Polygon {
    std::vector< gavrilova::Point > points;

    double area() const;
    bool isEven() const;
    bool isOdd() const;
    bool empty() const;
  };

  std::ostream& operator<<(std::ostream& os, const Polygon& polygon);
  std::istream& operator>>(std::istream& is, Polygon& polygon);

}

#endif
