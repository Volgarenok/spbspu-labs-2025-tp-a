#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <vector>
#include "point.h"

namespace kharlamov
{
  struct Polygon
  {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
  };
  std::istream& operator>>(std::istream& in, Polygon& poly);
}

#endif
