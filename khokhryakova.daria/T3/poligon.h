#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iostream>
#include "point.h"

namespace khokhryakova
{
  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };

  double getArea(const Polygon& poly);
  std::istream& operator>>(std::istream& in, Polygon& poly);
  std::ostream& operator<<(std::ostream& out, const Polygon& poly);
}

#endif
